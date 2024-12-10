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

struct coord
{
	double x;
	double y;
	double z;
	double w;
};

struct appstate
{
	SDL_Window* window;
	SDL_GLContext glcontext;
	GLuint shader;
	GLuint vbo;
	GLint a_position;
	GLint u_resolution;
	GLint u_time;
	GLint u_mouse;
	GLint u_coord;
	struct coord coord;
	int32_t window_width;
	int32_t window_height;
	uint64_t last_step;
};

struct appstate* appstate_init(void);
void appstate_free(struct appstate* ctx);

static int handle_key_event(struct appstate* ctx, SDL_Scancode key_code);
static bool compile_shader(char const* path, GLuint* shader, GLenum type);

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv);
SDL_AppResult SDL_AppIterate(void* appstate);
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event);
void SDL_AppQuit(void* appstate, SDL_AppResult result);

struct appstate* appstate_init(void)
{
	struct appstate* appstate = SDL_calloc(1, sizeof(*appstate));
	if (!appstate) return NULL;
	appstate->coord.x = 0;
	appstate->coord.y = 0;
	appstate->coord.z = 1;
	appstate->coord.w = 80;
	return appstate;
}

void appstate_free(struct appstate* ctx)
{
	if (!ctx) return;
	glDeleteProgram(ctx->shader);
	SDL_GL_DestroyContext(ctx->glcontext);
	SDL_DestroyWindow(ctx->window);
	SDL_free(ctx);
}

static int handle_key_event([[maybe_unused]] struct appstate* ctx, SDL_Scancode key_code)
{
	switch (key_code)
	{
	case SDL_SCANCODE_ESCAPE: [[fallthrough]];
	case SDL_SCANCODE_K: return SDL_APP_SUCCESS;
	// coord x
	case SDL_SCANCODE_LEFT: [[fallthrough]];
	case SDL_SCANCODE_A: ctx->coord.x -= .1 / ctx->coord.z; goto update_coord;
	case SDL_SCANCODE_RIGHT: [[fallthrough]];
	case SDL_SCANCODE_D: ctx->coord.x += .1 / ctx->coord.z; goto update_coord;
	// coord y
	case SDL_SCANCODE_DOWN: [[fallthrough]];
	case SDL_SCANCODE_S: ctx->coord.y -= .1 / ctx->coord.z; goto update_coord;
	case SDL_SCANCODE_UP: [[fallthrough]];
	case SDL_SCANCODE_W: ctx->coord.y += .1 / ctx->coord.z; goto update_coord;
	// coord zoom
	case SDL_SCANCODE_PAGEDOWN: [[fallthrough]];
	case SDL_SCANCODE_E: ctx->coord.z -= ctx->coord.z / 100.; if (ctx->coord.z <= 0.) ctx->coord.z = 1e-100; goto update_coord;
	case SDL_SCANCODE_PAGEUP: [[fallthrough]];
	case SDL_SCANCODE_Q: ctx->coord.z += ctx->coord.z / 100.; goto update_coord;
	// coord max_iteration
	case SDL_SCANCODE_F: ctx->coord.w -= ctx->coord.w / 100.; goto update_coord;
	case SDL_SCANCODE_R: ctx->coord.w += ctx->coord.w / 100.; goto update_coord;
	default: break;
	}
	return SDL_APP_CONTINUE;
update_coord:
	if (ctx->u_coord != -1)
	{
		glUseProgram(ctx->shader);
		glUniform4f(ctx->u_coord, ctx->coord.x, ctx->coord.y, ctx->coord.z, ctx->coord.w);
		glUseProgram(0);
	}
	return SDL_APP_CONTINUE;
}

static bool compile_shader(char const* path, GLuint* shader, GLenum type)
{
	FILE* file = fopen(path, "rb");
	if (!file)
	{
		SDL_LogError_strerror(fopen);
		return false;
	}
	if (fseek(file, 0, SEEK_END) == -1) return false;
	long size = ftell(file);
	if (size == -1) return false;
	if (fseek(file, 0, SEEK_SET) == -1) return false;
	GLchar* buf = SDL_malloc((size + 1) * sizeof(GLchar));
	if (fread(buf, sizeof(GLchar), size, file) != (size_t)size)
	{
		SDL_free(buf);
		return false;
	}
	fclose(file);
	buf[size] = '\0';
	GLuint frag = glCreateShader(type);
	GLchar const* cbuf = buf;
	glShaderSource(frag, 1, &cbuf, NULL);
	glCompileShader(frag);
	SDL_free(buf);
	GLint success;
	glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLsizei errsize = 0;
		glGetShaderiv(frag, GL_INFO_LOG_LENGTH, &errsize);
		char* errbuf = calloc(errsize + 1, sizeof(char));
		glGetShaderInfoLog(frag, errsize, NULL, errbuf);
		SDL_LogError_("shader program compilation failed: %s", errbuf);
		free(errbuf);
		return false;
	}
	*shader = frag;
	return true;
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

	if (!(*appstate = appstate_init())) return SDL_APP_FAILURE;
	struct appstate* ctx = *appstate;

	ctx->window_width = SDL_WINDOW_WIDTH;
	ctx->window_height = SDL_WINDOW_HEIGHT;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	if (!(ctx->window = SDL_CreateWindow("Example SDL3 Program", ctx->window_width, ctx->window_height,
										 SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN)))
	{
		SDL_LogError_GetError(SDL_CreateWindow);
		return SDL_APP_FAILURE;
	}

	if (!(ctx->glcontext = SDL_GL_CreateContext(ctx->window)))
	{
		SDL_LogError_GetError(SDL_GL_GetCurrentContext);
		return SDL_APP_FAILURE;
	}

	GLuint vert;
	if (!compile_shader(vert_file, &vert, GL_VERTEX_SHADER)) return SDL_APP_FAILURE;

	GLuint frag;
	if (!compile_shader(frag_file, &frag, GL_FRAGMENT_SHADER)) return SDL_APP_FAILURE;

	ctx->shader = glCreateProgram();
	glAttachShader(ctx->shader, vert);
	glAttachShader(ctx->shader, frag);
	glLinkProgram(ctx->shader);
	glDetachShader(ctx->shader, vert);
	glDetachShader(ctx->shader, frag);
	glDeleteShader(vert);
	glDeleteShader(frag);
	GLint success;
	glGetProgramiv(ctx->shader, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLsizei errsize = 0;
		glGetProgramiv(ctx->shader, GL_INFO_LOG_LENGTH, &errsize);
		char* errbuf = calloc(errsize + 1, sizeof(char));
		glGetProgramInfoLog(ctx->shader, errsize, NULL, errbuf);
		SDL_LogError_("shader program linking failed: %s", errbuf);
		free(errbuf);
		return SDL_APP_FAILURE;
	}
	glUseProgram(ctx->shader);

	static GLfloat points[] = { 1, -1, -1, -1, -1, 1, 1, 1 };
	glGenBuffers(1, &ctx->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, ctx->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	ctx->a_position = glGetAttribLocation(ctx->shader, "a_position");
	if (ctx->a_position == -1)
	{
		SDL_LogError_glGetError(glGetAttribLocation);
		return SDL_APP_FAILURE;
	}
	glVertexAttribPointer(ctx->a_position, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(ctx->a_position);

	ctx->u_resolution = glGetUniformLocation(ctx->shader, "u_resolution");
	if (ctx->u_resolution != -1) glUniform2f(ctx->u_resolution, ctx->window_width, ctx->window_height);

	ctx->u_time = glGetUniformLocation(ctx->shader, "u_time");
	if (ctx->u_time != -1) glUniform1f(ctx->u_time, SDL_GetTicks() / 1000.f);

	ctx->u_mouse = glGetUniformLocation(ctx->shader, "u_mouse");
	if (ctx->u_mouse != -1) glUniform2f(ctx->u_mouse, 0, 0);

	ctx->u_coord = glGetUniformLocation(ctx->shader, "u_coord");
	if (ctx->u_coord != -1) glUniform4f(ctx->u_coord, ctx->coord.x, ctx->coord.y, ctx->coord.z, ctx->coord.w);

	glUseProgram(0);
	glClearColor(0, 0, 0, 1);
	glViewport(0, 0, ctx->window_width, ctx->window_height);
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CW); // GL_CCW for counter clock-wise

	ctx->last_step = SDL_GetTicks();

	SDL_ShowWindow(ctx->window);
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
	struct appstate* ctx = appstate;

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(ctx->shader);
	glBindBuffer(GL_ARRAY_BUFFER, ctx->vbo);
	glEnableVertexAttribArray(ctx->a_position);

	if (ctx->u_time != -1) glUniform1f(ctx->u_time, SDL_GetTicks() / 1000.f);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glUseProgram(0);

	SDL_GL_SwapWindow(ctx->window);

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	struct appstate* ctx = appstate;
	switch (event->type)
	{
	case SDL_EVENT_QUIT: return SDL_APP_SUCCESS;
	case SDL_EVENT_KEY_DOWN: return handle_key_event(ctx, event->key.scancode);
	case SDL_EVENT_WINDOW_RESIZED:
		ctx->window_width = event->window.data1;
		ctx->window_height = event->window.data2;
		if (ctx->u_resolution != -1) glUniform2f(ctx->u_resolution, ctx->window_width, ctx->window_height);
		glUseProgram(ctx->shader);
		glUseProgram(0);
		glViewport(0, 0, ctx->window_width, ctx->window_height);
		break;
	case SDL_EVENT_MOUSE_MOTION:
		if (event->motion.state & SDL_BUTTON_LEFT && ctx->u_mouse != -1)
		{
			glUseProgram(ctx->shader);
			glUniform2f(ctx->u_mouse, event->motion.x, event->motion.y);
			glUseProgram(0);
		}
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

