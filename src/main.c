// sdl3
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_opengles2.h>
// libc
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SDL_WINDOW_WIDTH 1280
#define SDL_WINDOW_HEIGHT 720

#define SDL_LogError_(...) SDL_LogError(SDL_LOG_CATEGORY_ERROR, __VA_ARGS__)
#define SDL_LogError_GetError(F) SDL_LogError_(#F " failed: %s\n", SDL_GetError())
#define SDL_LogError_strerror(F) SDL_LogError_(#F " failed: %s\n", strerror(errno))
#define SDL_LogError_glGetError(F) SDL_LogError_(#F " failed: %x\n", glGetError())

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

struct window
{
	SDL_Window* window;
	struct context ctx;
	bool fullscreen;
	// uniforms
	struct vec4 coord;
	struct vec2 resolution;
	struct vec2 mouse;
	double time;
};

struct appstate
{
	struct window* window;
};

struct appstate* appstate_init(void);
void appstate_free(struct appstate* ctx);

void window_draw(struct window* window);
bool window_init_gles2(struct window* window, char const* vert_path, char const* frag_path);
struct window* window_init(enum context_type type, char const* vert_path, char const* frag_path);
void window_free(struct window* window);

GLuint gles2_compile_shader(char const* path, GLenum type);
GLuint gles2_compile_program(char const* vert_path, char const* frag_path);
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
	SDL_free(ctx);
}

void window_draw(struct window* window)
{
	window->time = SDL_GetTicks() / 1000.;
	switch (window->ctx.type)
	{
	case CONTEXT_GLES2: gles2_draw_program(window); break;
	};
}

void window_update_resolution(struct window* window, int width, int height)
{
	if (window->resolution.x == width && window->resolution.y == height) return;
	window->resolution.x = width;
	window->resolution.y = height;
	switch (window->ctx.type)
	{
	case CONTEXT_GLES2: glViewport(0, 0, window->resolution.x, window->resolution.y); break;
	};
}

bool window_init_gles2(struct window* window, char const* vert_path, char const* frag_path)
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	if (!(window->window = SDL_CreateWindow("Example SDL3 Program", SDL_WINDOW_WIDTH, SDL_WINDOW_HEIGHT,
											SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN)))
	{
		SDL_LogError_GetError(SDL_CreateWindow);
		return false;
	}

	int width = 0;
	int height = 0;
	if (!SDL_GetWindowSizeInPixels(window->window, &width, &height))
	{
		SDL_LogError_GetError(SDL_GetWindowSizeInPixels);
		return false;
	}
	window->resolution = (struct vec2){ width, height };

	if (!(window->ctx.value.gles2 = gles2_init(window, vert_path, frag_path))) return false;

	return window;
}

struct window* window_init(enum context_type type, char const* vert_path, char const* frag_path)
{
	struct window* window = SDL_calloc(1, sizeof(*window));
	if (!window) return NULL;
	window->ctx.type = type;
	window->coord.x = 0;
	window->coord.y = 0;
	window->coord.z = 1;
	window->coord.w = 80;
	window->time = SDL_GetTicks();

	switch (type)
	{
	case CONTEXT_GLES2:
		if (!window_init_gles2(window, vert_path, frag_path)) goto failure;
		break;
	};

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
	SDL_DestroyWindow(window->window);
	SDL_free(window);
}

struct gles2_context* gles2_init(struct window* window, char const* vert_path, char const* frag_path)
{
	struct gles2_context* ctx = SDL_calloc(1, sizeof(*ctx));
	if (!ctx) return NULL;
	ctx->window = window->window;
	if (!(ctx->gl = SDL_GL_CreateContext(window->window)))
	{
		SDL_LogError_GetError(SDL_GL_GetCurrentContext);
		goto failure;
	}
	SDL_GL_MakeCurrent(window->window, ctx->gl);
	glClearColor(0, 0, 0, 1);
	glViewport(0, 0, window->resolution.x, window->resolution.y);
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CW); // GL_CCW for counter clock-wise

	ctx->prog = gles2_compile_program(vert_path, frag_path);
	gles2_init_program(ctx);
	return ctx;
failure:
	gles2_free(ctx);
	return NULL;
}

GLuint gles2_compile_shader(char const* path, GLenum type)
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
	GLuint shad = glCreateShader(type);
	GLchar const* cbuf = buf;
	glShaderSource(shad, 1, &cbuf, NULL);
	glCompileShader(shad);
	SDL_free(buf);
	GLint success;
	glGetShaderiv(shad, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLsizei errsize = 0;
		glGetShaderiv(shad, GL_INFO_LOG_LENGTH, &errsize);
		char* errbuf = calloc(errsize + 1, sizeof(char));
		glGetShaderInfoLog(shad, errsize, NULL, errbuf);
		SDL_LogError_("shader program compilation failed: %s", errbuf);
		free(errbuf);
		return 0;
	}
	return shad;
}

GLuint gles2_compile_program(char const* vert_path, char const* frag_path)
{
	GLuint vert = gles2_compile_shader(vert_path, GL_VERTEX_SHADER);
	if (!vert) return 0;
	GLuint frag = gles2_compile_shader(frag_path, GL_FRAGMENT_SHADER);
	if (!frag)
	{
		glDeleteShader(vert);
		return 0;
	}
	GLuint prog = glCreateProgram();
	if (!prog)
	{
		glDeleteShader(vert);
		glDeleteShader(frag);
		return 0;
	}
	glAttachShader(prog, vert);
	glAttachShader(prog, frag);
	glLinkProgram(prog);
	glDetachShader(prog, vert);
	glDetachShader(prog, frag);
	glDeleteShader(vert);
	glDeleteShader(frag);
	GLint success;
	glGetProgramiv(prog, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLsizei errsize = 0;
		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &errsize);
		char* errbuf = calloc(errsize + 1, sizeof(char));
		glGetProgramInfoLog(prog, errsize, NULL, errbuf);
		SDL_LogError_("shader program linking failed: %s", errbuf);
		free(errbuf);
		return 0;
	}
	return prog;
}

bool gles2_init_program(struct gles2_context* ctx)
{
	glUseProgram(ctx->prog);
	GLfloat points[] = { 1, -1, -1, -1, -1, 1, 1, 1 };
	glGenBuffers(1, &ctx->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, ctx->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	ctx->a_position = glGetAttribLocation(ctx->prog, "a_position");
	if (ctx->a_position == -1)
	{
		SDL_LogError_glGetError(glGetAttribLocation);
		return false;
	}
	glVertexAttribPointer(ctx->a_position, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	ctx->u_resolution = glGetUniformLocation(ctx->prog, "u_resolution");
	ctx->u_time = glGetUniformLocation(ctx->prog, "u_time");
	ctx->u_mouse = glGetUniformLocation(ctx->prog, "u_mouse");
	ctx->u_coord = glGetUniformLocation(ctx->prog, "u_coord");
	glUseProgram(0);
	return true;
}

void gles2_update_program(struct window* window)
{
	struct gles2_context* ctx = window->ctx.value.gles2;
	if (ctx->u_resolution != -1) glUniform2f(ctx->u_resolution, window->resolution.x, window->resolution.y);
	if (ctx->u_time != -1) glUniform1f(ctx->u_time, window->time);
	if (ctx->u_mouse != -1) glUniform2f(ctx->u_mouse, window->mouse.x, window->mouse.y);
	if (ctx->u_coord != -1)
		glUniform4f(ctx->u_coord, window->coord.x, window->coord.y, window->coord.z, window->coord.w);
}

void gles2_draw_program(struct window* window)
{
	struct gles2_context* ctx = window->ctx.value.gles2;
	SDL_GL_MakeCurrent(window->window, ctx->gl);

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(ctx->prog);
	gles2_update_program(window);
	glBindBuffer(GL_ARRAY_BUFFER, ctx->vbo);
	glEnableVertexAttribArray(ctx->a_position);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDisableVertexAttribArray(ctx->a_position);
	glUseProgram(0);

	SDL_GL_SwapWindow(ctx->window);
}

void gles2_free(struct gles2_context* ctx)
{
	if (!ctx) return;
	if (ctx->gl)
	{
		SDL_GL_MakeCurrent(ctx->window, ctx->gl);
		if (ctx->prog) glDeleteProgram(ctx->prog);
		if (ctx->vbo != GL_INVALID_VALUE) glDeleteBuffers(1, &ctx->vbo);
		SDL_GL_DestroyContext(ctx->gl);
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

	char const* vert_file = argc >= 3 ? argv[2] : "files/base.vert";
	char const* frag_file = argc >= 2 ? argv[1] : "files/mandelbrot.frag";

	if (!SDL_SetAppMetadata("Example SDL3 Program", "1.0.0", "fr.vokunaav.sdl3"))
	{
		SDL_LogError_GetError(SDL_SetAppMetadata);
		return SDL_APP_FAILURE;
	}

	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_LogError_GetError(SDL_Init);
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

	ctx->window = window_init(CONTEXT_GLES2, vert_file, frag_file);
	if (!ctx->window)
		return SDL_APP_FAILURE;

	SDL_ShowWindow(ctx->window->window);
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
	case SDL_EVENT_MOUSE_MOTION:
		if (event->motion.windowID != SDL_GetWindowID(ctx->window->window)) break;
		if (event->motion.state & SDL_BUTTON_LEFT)
		{
			ctx->window->mouse.x = event->motion.x;
			ctx->window->mouse.y = event->motion.y;
		}
		break;
	case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
		if (event->motion.windowID != SDL_GetWindowID(ctx->window->window)) break;
		ctx->window->fullscreen = true;
		break;
	case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
		if (event->motion.windowID != SDL_GetWindowID(ctx->window->window)) break;
		ctx->window->fullscreen = false;
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

