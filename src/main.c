// SDL3
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
// SDL3_ttf
#include <SDL3_ttf/SDL_ttf.h>
// libc
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// shader-viewer
#include "gles2.h"

#ifdef _WIN32
#	pragma comment(lib, "SDL3.lib")
#	pragma comment(lib, "SDL3_ttf.lib")
#	define SEP "\\"
#else
#	define SEP "/"
#endif

#define SDL_WINDOW_WIDTH 1280
#define SDL_WINDOW_HEIGHT 720

#define SDL_DEFAULT_VERT "files" SEP "base.vert"
#define SDL_DEFAULT_FRAG "files" SEP "mandelbrot.frag"
#define SDL_DEFAULT_FONT "files" SEP "BigBlueTermPlusNerdFontMono-Regular.ttf"

#define SDL_LogError_(...) SDL_LogError(SDL_LOG_CATEGORY_ERROR, __VA_ARGS__)
#define SDL_LogError_GetError(F) SDL_LogError_(#F " failed: %s\n", SDL_GetError())
#define SDL_LogError_strerror(F) SDL_LogError_(#F " failed: %s\n", strerror(errno))
#define SDL_LogError_glGetError(F) SDL_LogError_(#F " failed: %x\n", ctx->glGetError())

#define SDL_LogInfo_(...) SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__)

struct vec2
{
	double x;
	double y;
};

struct vec4
{
	double x;
	double y;
	double z;
	double w;
};

struct gles2_context
{
	SDL_Window* window;
	SDL_GLContext gl;
	GLuint prog;
	GLuint vbo;
	GLint a_position;
	GLint u_resolution;
	GLint u_time;
	GLint u_mouse;
	GLint u_coord;

	GL_DYNLOAD_ATTRS();
};

enum context_type
{
	CONTEXT_GLES2,
};

struct context
{
	enum context_type type;
	union context_value
	{
		struct gles2_context* gles2;
	} value;
};

struct appstate;

struct window
{
	struct appstate* app;
	SDL_Window* window;
	SDL_Renderer* renderer;
	struct context ctx;
	bool fullscreen;
	double frames[60];
	size_t frames_idx;
	size_t frames_size;
	bool display_info;
	// uniforms
	struct vec4 coord;
	struct vec2 resolution;
	struct vec2 mouse;
	double time;
};

struct appstate
{
	struct window* window;
	TTF_Font* font;
	char const* vert_path;
	char const* frag_path;
};

struct appstate* appstate_init(void);
void appstate_free(struct appstate* ctx);

void window_draw(struct window* window);
bool window_init_gles2(struct window* window, char const* vert_path, char const* frag_path);
struct window* window_init(struct appstate* app, enum context_type type, char const* vert_path, char const* frag_path);
void window_free(struct window* window);

GLuint gles2_compile_shader(struct gles2_context* ctx, char const* path, GLenum type);
GLuint gles2_compile_program(struct gles2_context* ctx, char const* vert_path, char const* frag_path);
bool gles2_init_program(struct gles2_context* ctx);
void gles2_update_program(struct window* window);
void gles2_draw_program(struct window* window);
struct gles2_context* gles2_init(struct window* window, char const* vert_path, char const* frag_path);
void gles2_free(struct gles2_context* ctx);

static int handle_key_event(struct window* window, SDL_Scancode key_code);

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv);
SDL_AppResult SDL_AppIterate(void* appstate);
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event);
void SDL_AppQuit(void* appstate, SDL_AppResult result);

struct appstate* appstate_init(void)
{
	struct appstate* ctx = SDL_calloc(1, sizeof(*ctx));
	return ctx;
}

void appstate_free(struct appstate* ctx)
{
	if (!ctx) return;
	window_free(ctx->window);
	TTF_CloseFont(ctx->font);
	SDL_free(ctx);
}

void window_draw(struct window* window)
{
	double old_time = window->time;
	window->time = SDL_GetTicks() / 1000.;
	SDL_SetRenderDrawColor(window->renderer, 0x18, 0x18, 0x18, 0xff);
	SDL_RenderClear(window->renderer);
	SDL_FlushRenderer(window->renderer);
	// needed for webgl
	window->ctx.value.gles2->glDisableVertexAttribArray(1);
	switch (window->ctx.type)
	{
	case CONTEXT_GLES2: gles2_draw_program(window); break;
	};
	// needed for webgl
	window->ctx.value.gles2->glEnableVertexAttribArray(1);

	// fps
	window->frames_idx = (window->frames_idx + 1) % SDL_arraysize(window->frames);
	window->frames[window->frames_idx] = window->time - old_time;
	if (window->frames_size < SDL_arraysize(window->frames)) ++window->frames_size;
	if (window->display_info)
	{
		double frames_sum = 0.;
		for (size_t i = 0; i < window->frames_size; ++i)
			frames_sum += window->frames[i];
		double fps = 1 / (frames_sum / window->frames_size);
		char fps_buffer[256];
		snprintf(fps_buffer, sizeof(fps_buffer), "%.2f", fps);
		SDL_Color fps_color = { 0xee, 0xee, 0xee, 0xff };
		SDL_Surface* fps_surface = TTF_RenderText_Solid(window->app->font, fps_buffer, 0, fps_color);
		SDL_Texture* fps_texture = SDL_CreateTextureFromSurface(window->renderer, fps_surface);
		SDL_FRect fps_rect = { 10, 10, fps_surface->w + 10, fps_surface->w + 10 };
		if (!SDL_RenderTexture(window->renderer, fps_texture, NULL, &fps_rect))
			SDL_LogError_GetError(SDL_RenderTexture);
		SDL_DestroySurface(fps_surface);
		SDL_DestroyTexture(fps_texture);
	}

	SDL_RenderPresent(window->renderer);
}

void gles2_draw_program(struct window* window)
{
	struct gles2_context* ctx = window->ctx.value.gles2;
	SDL_GL_MakeCurrent(window->window, ctx->gl);

	GLint old_prog = 0;
	GLint old_vbo = 0;
	ctx->glGetIntegerv(GL_CURRENT_PROGRAM, &old_prog);
	ctx->glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &old_vbo);

	ctx->glUseProgram(ctx->prog);
	ctx->glBindBuffer(GL_ARRAY_BUFFER, ctx->vbo);
	ctx->glVertexAttribPointer(ctx->a_position, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	ctx->glEnableVertexAttribArray(ctx->a_position);

	gles2_update_program(window);

	ctx->glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	ctx->glBindBuffer(GL_ARRAY_BUFFER, old_vbo);
	ctx->glUseProgram(old_prog);
}

void window_update_resolution(struct window* window, int width, int height)
{
	if (window->resolution.x == width && window->resolution.y == height) return;
	window->resolution.x = width;
	window->resolution.y = height;
	SDL_Rect rect = { 0, 0, window->resolution.x, window->resolution.y };
	SDL_SetRenderViewport(window->renderer, &rect);
}

bool window_init_gles2(struct window* window, [[maybe_unused]] char const* vert_path,
					   [[maybe_unused]] char const* frag_path)
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	if (!(window->window =
			  SDL_CreateWindow("shader-viewer", SDL_WINDOW_WIDTH, SDL_WINDOW_HEIGHT,
							   SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN | SDL_WINDOW_BORDERLESS)))
	{
		SDL_LogError_GetError(SDL_CreateWindow);
		return false;
	}

	SDL_PropertiesID props = SDL_CreateProperties();
	SDL_SetStringProperty(props, SDL_PROP_RENDERER_CREATE_NAME_STRING, "opengles2");
	SDL_SetPointerProperty(props, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, window->window);
	SDL_SetNumberProperty(props, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, SDL_RENDERER_VSYNC_ADAPTIVE);
	window->renderer = SDL_CreateRendererWithProperties(props);
	SDL_DestroyProperties(props);
	if (!window->renderer)
	{
		SDL_LogError_GetError(SDL_CreateRendererWithProperties);
		return false;
	}
	SDL_FlushRenderer(window->renderer);

	if (!(window->ctx.value.gles2 = gles2_init(window, vert_path, frag_path))) return false;

	return window;
}

struct window* window_init(struct appstate* app, enum context_type type, char const* vert_path, char const* frag_path)
{
	struct window* window = SDL_calloc(1, sizeof(*window));
	if (!window) return NULL;
	window->app = app;
	window->ctx.type = type;
	window->coord.x = 0;
	window->coord.y = 0;
	window->coord.z = 1;
	window->coord.w = 80;
	window->time = SDL_GetTicks() / 1000.;
	for (size_t i = 0; i < SDL_arraysize(window->frames); ++i)
		window->frames[i] = 0.;

	switch (type)
	{
	case CONTEXT_GLES2:
		if (!window_init_gles2(window, vert_path, frag_path)) goto failure;
		break;
	};

	int width = 0;
	int height = 0;
	if (!SDL_GetWindowSizeInPixels(window->window, &width, &height))
	{
		SDL_LogError_GetError(SDL_GetWindowSizeInPixels);
		goto failure;
	}
	window->resolution = (struct vec2){ width, height };

	return window;
failure:
	window_free(window);
	return NULL;
}

void window_free(struct window* window)
{
	if (!window) return;
	switch (window->ctx.type)
	{
	case CONTEXT_GLES2: gles2_free(window->ctx.value.gles2); break;
	};
	SDL_DestroyRenderer(window->renderer);
	SDL_DestroyWindow(window->window);
	SDL_free(window);
}

struct gles2_context* gles2_init(struct window* window, char const* vert_path, char const* frag_path)
{
	struct gles2_context* ctx = SDL_calloc(1, sizeof(*ctx));
	if (!ctx) return NULL;
	ctx->window = window->window;
	if (!(ctx->gl = SDL_GL_GetCurrentContext()))
	{
		SDL_LogError_GetError(SDL_GL_GetCurrentContext);
		goto failure;
	}

	GL_DYNLOAD_LOADS();

	ctx->glEnable(GL_CULL_FACE); // cull face
	ctx->glCullFace(GL_BACK); // cull back face
	ctx->glFrontFace(GL_CW); // GL_CCW for counter clock-wise

	ctx->prog = gles2_compile_program(ctx, vert_path, frag_path);
	if (!ctx->prog) goto failure;
	if (!gles2_init_program(ctx)) goto failure;
	return ctx;
failure:
	gles2_free(ctx);
	return NULL;
}

GLuint gles2_compile_shader(struct gles2_context* ctx, char const* path, GLenum type)
{
	FILE* file = fopen(path, "rb");
	if (!file)
	{
		SDL_LogError_strerror(fopen);
		return 0;
	}
	if (fseek(file, 0, SEEK_END) == -1)
	{
		SDL_LogError_strerror(fseek);
		fclose(file);
		return 0;
	}
	long size = ftell(file);
	if (size == -1)
	{
		SDL_LogError_strerror(ftell);
		fclose(file);
		return false;
	}
	if (fseek(file, 0, SEEK_SET) == -1)
	{
		SDL_LogError_strerror(fseek);
		fclose(file);
		return 0;
	}
	GLchar* buf = SDL_malloc((size + 1) * sizeof(GLchar));
	if (!buf)
	{
		SDL_LogError_strerror(fread);
		return 0;
	}
	if (fread(buf, sizeof(GLchar), size, file) != (size_t)size)
	{
		SDL_LogError_strerror(fread);
		SDL_free(buf);
		fclose(file);
		return 0;
	}
	fclose(file);
	buf[size] = '\0';
	GLuint shad = ctx->glCreateShader(type);
	GLchar const* cbuf = buf;
	ctx->glShaderSource(shad, 1, &cbuf, NULL);
	ctx->glCompileShader(shad);
	SDL_free(buf);
	GLint success;
	ctx->glGetShaderiv(shad, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLsizei errsize = 0;
		ctx->glGetShaderiv(shad, GL_INFO_LOG_LENGTH, &errsize);
		char* errbuf = calloc(errsize + 1, sizeof(char));
		ctx->glGetShaderInfoLog(shad, errsize, NULL, errbuf);
		SDL_LogError_("shader program compilation failed: %s", errbuf);
		free(errbuf);
		return 0;
	}
	return shad;
}

GLuint gles2_compile_program(struct gles2_context* ctx, char const* vert_path, char const* frag_path)
{
	GLuint vert = gles2_compile_shader(ctx, vert_path, GL_VERTEX_SHADER);
	if (!vert) return 0;
	GLuint frag = gles2_compile_shader(ctx, frag_path, GL_FRAGMENT_SHADER);
	if (!frag)
	{
		ctx->glDeleteShader(vert);
		return 0;
	}
	GLuint prog = ctx->glCreateProgram();
	if (!prog)
	{
		ctx->glDeleteShader(vert);
		ctx->glDeleteShader(frag);
		return 0;
	}
	ctx->glAttachShader(prog, vert);
	ctx->glAttachShader(prog, frag);
	ctx->glLinkProgram(prog);
	ctx->glDetachShader(prog, vert);
	ctx->glDetachShader(prog, frag);
	ctx->glDeleteShader(vert);
	ctx->glDeleteShader(frag);
	GLint success;
	ctx->glGetProgramiv(prog, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLsizei errsize = 0;
		ctx->glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &errsize);
		char* errbuf = calloc(errsize + 1, sizeof(char));
		ctx->glGetProgramInfoLog(prog, errsize, NULL, errbuf);
		SDL_LogError_("shader program linking failed: %s", errbuf);
		free(errbuf);
		return 0;
	}
	return prog;
}

bool gles2_init_program(struct gles2_context* ctx)
{
	GLint old_prog = 0;
	ctx->glGetIntegerv(GL_CURRENT_PROGRAM, &old_prog);
	ctx->glUseProgram(ctx->prog);

	ctx->a_position = ctx->glGetAttribLocation(ctx->prog, "a_position");
	if (ctx->a_position == -1)
	{
		SDL_LogError_glGetError(glGetAttribLocation);
		return false;
	}

	ctx->u_resolution = ctx->glGetUniformLocation(ctx->prog, "u_resolution");
	ctx->u_time = ctx->glGetUniformLocation(ctx->prog, "u_time");
	ctx->u_mouse = ctx->glGetUniformLocation(ctx->prog, "u_mouse");
	ctx->u_coord = ctx->glGetUniformLocation(ctx->prog, "u_coord");

	GLfloat points[] = { 1, -1, -1, -1, -1, 1, 1, 1 };
	ctx->glGenBuffers(1, &ctx->vbo);
	if (ctx->vbo == GL_INVALID_VALUE)
	{
		SDL_LogError_glGetError(glGenBuffers);
		return false;
	}
	GLint old_vbo = 0;
	ctx->glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &old_vbo);
	ctx->glBindBuffer(GL_ARRAY_BUFFER, ctx->vbo);
	ctx->glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	ctx->glBindBuffer(GL_ARRAY_BUFFER, old_vbo);

	ctx->glUseProgram(old_prog);
	return true;
}

void gles2_update_program(struct window* window)
{
	struct gles2_context* ctx = window->ctx.value.gles2;
	if (ctx->u_resolution != -1) ctx->glUniform2f(ctx->u_resolution, window->resolution.x, window->resolution.y);
	if (ctx->u_time != -1) ctx->glUniform1f(ctx->u_time, window->time);
	if (ctx->u_mouse != -1) ctx->glUniform2f(ctx->u_mouse, window->mouse.x, window->mouse.y);
	if (ctx->u_coord != -1)
		ctx->glUniform4f(ctx->u_coord, window->coord.x, window->coord.y, window->coord.z, window->coord.w);
}

void gles2_free(struct gles2_context* ctx)
{
	if (!ctx) return;
	if (ctx->gl)
	{
		SDL_GL_MakeCurrent(ctx->window, ctx->gl);
		if (ctx->prog) ctx->glDeleteProgram(ctx->prog);
		if (ctx->vbo != GL_INVALID_VALUE) ctx->glDeleteBuffers(1, &ctx->vbo);
	}
	SDL_free(ctx);
}

static int handle_key_event(struct window* window, SDL_Scancode key_code)
{
	switch (key_code)
	{
	// quit
	case SDL_SCANCODE_ESCAPE: [[fallthrough]];
	case SDL_SCANCODE_K: return SDL_APP_SUCCESS;
	// toggle fullscreen
	case SDL_SCANCODE_L: SDL_SetWindowFullscreen(window->window, !window->fullscreen); break;
	// toggle display info
	case SDL_SCANCODE_H: window->display_info = !window->display_info; break;
	// coord x
	case SDL_SCANCODE_LEFT: [[fallthrough]];
	case SDL_SCANCODE_A: window->coord.x -= .1 / window->coord.z; break;
	case SDL_SCANCODE_RIGHT: [[fallthrough]];
	case SDL_SCANCODE_D: window->coord.x += .1 / window->coord.z; break;
	// coord y
	case SDL_SCANCODE_DOWN: [[fallthrough]];
	case SDL_SCANCODE_S: window->coord.y -= .1 / window->coord.z; break;
	case SDL_SCANCODE_UP: [[fallthrough]];
	case SDL_SCANCODE_W: window->coord.y += .1 / window->coord.z; break;
	// coord zoom
	case SDL_SCANCODE_PAGEDOWN: [[fallthrough]];
	case SDL_SCANCODE_E:
		window->coord.z -= window->coord.z / 100.;
		if (window->coord.z <= 0.) window->coord.z = 1e-100;
		break;
	case SDL_SCANCODE_PAGEUP: [[fallthrough]];
	case SDL_SCANCODE_Q: window->coord.z += window->coord.z / 100.; break;
	// coord max_iteration
	case SDL_SCANCODE_F: window->coord.w -= window->coord.w / 100.; break;
	case SDL_SCANCODE_R: window->coord.w += window->coord.w / 100.; break;
	default: break;
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv)
{
	if (argc < 1 || 3 < argc)
	{
		SDL_LogError_("incorrect number of arguments %i\n", argc);
		return SDL_APP_FAILURE;
	}

	if (!SDL_SetAppMetadata("shader-viewer", "1.0.0", "fr.vokunaav.shader-viewer"))
	{
		SDL_LogError_GetError(SDL_SetAppMetadata);
		return SDL_APP_FAILURE;
	}

	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_LogError_GetError(SDL_Init);
		return SDL_APP_FAILURE;
	}

	if (!TTF_Init())
	{
		SDL_LogError_GetError(TTF_Init);
		return SDL_APP_FAILURE;
	}

	const struct
	{
		char const* key;
		char const* value;
	} extended_metadata[] = {
		{ SDL_PROP_APP_METADATA_CREATOR_STRING,	"Maxence MICHOT"				 },
		{ SDL_PROP_APP_METADATA_COPYRIGHT_STRING, "Placed under the MIT license" },
		{ SDL_PROP_APP_METADATA_TYPE_STRING,		 "game"							},
	};

	for (size_t i = 0; i < SDL_arraysize(extended_metadata); ++i)
	{
		if (!SDL_SetAppMetadataProperty(extended_metadata[i].key, extended_metadata[i].value))
		{
			SDL_LogError_GetError(SDL_SetAppMetadataProperty);
			return SDL_APP_FAILURE;
		}
	}

	struct appstate* ctx = *appstate = appstate_init();
	if (!ctx) return SDL_APP_FAILURE;

	ctx->vert_path = argc >= 3 ? argv[2] : SDL_DEFAULT_VERT;
	ctx->frag_path = argc >= 2 ? argv[1] : SDL_DEFAULT_FRAG;

	ctx->window = window_init(ctx, CONTEXT_GLES2, ctx->vert_path, ctx->frag_path);
	if (!ctx->window) return SDL_APP_FAILURE;

	SDL_ShowWindow(ctx->window->window);

	ctx->font = TTF_OpenFont(SDL_DEFAULT_FONT, 24);
	if (!ctx->font)
	{
		SDL_LogError_GetError(TTF_OpenFont);
		return SDL_APP_FAILURE;
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
	struct appstate* ctx = appstate;
	window_draw(ctx->window);
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	struct appstate* ctx = appstate;
	switch (event->type)
	{
	case SDL_EVENT_QUIT: return SDL_APP_SUCCESS;
	case SDL_EVENT_KEY_DOWN:
		if (event->key.windowID != SDL_GetWindowID(ctx->window->window)) break;
		return handle_key_event(ctx->window, event->key.scancode);
	case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
		if (event->window.windowID != SDL_GetWindowID(ctx->window->window)) break;
		window_update_resolution(ctx->window, event->window.data1, event->window.data2);
		break;
	case SDL_EVENT_WINDOW_RESIZED:
		if (event->window.windowID != SDL_GetWindowID(ctx->window->window)) break;
		window_update_resolution(ctx->window, event->window.data1, event->window.data2);
		break;
	case SDL_EVENT_MOUSE_MOTION:
		if (event->motion.windowID != SDL_GetWindowID(ctx->window->window)) break;
		if (event->motion.state & SDL_BUTTON_LEFT)
		{
			ctx->window->mouse.x = event->motion.x;
			ctx->window->mouse.y = event->motion.y;
		}
		break;
	case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
		if (event->window.windowID != SDL_GetWindowID(ctx->window->window)) break;
		ctx->window->fullscreen = true;
		break;
	case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
		if (event->window.windowID != SDL_GetWindowID(ctx->window->window)) break;
		ctx->window->fullscreen = false;
		break;
	case SDL_EVENT_DROP_FILE:
		if (event->drop.windowID != SDL_GetWindowID(ctx->window->window)) break;
		SDL_LogInfo_("file dropped: %s\n", event->drop.data);
		struct gles2_context* gl = ctx->window->ctx.value.gles2;
		SDL_FlushRenderer(ctx->window->renderer);
		SDL_GL_MakeCurrent(gl->window, gl->gl);
		// needed for webgl
		ctx->window->ctx.value.gles2->glDisableVertexAttribArray(1);
		if (gl->prog) ctx->window->ctx.value.gles2->glDeleteProgram(gl->prog);
		if (gl->vbo != GL_INVALID_VALUE) ctx->window->ctx.value.gles2->glDeleteBuffers(1, &gl->vbo);
		gl->prog = gles2_compile_program(gl, ctx->vert_path, event->drop.data);
		gles2_init_program(gl);
		// needed for webgl
		ctx->window->ctx.value.gles2->glEnableVertexAttribArray(1);
		break;
	default: break;
	}
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, [[maybe_unused]] SDL_AppResult result)
{
	struct appstate* ctx = appstate;
	appstate_free(ctx);
}

