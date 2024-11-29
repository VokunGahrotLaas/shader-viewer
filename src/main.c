// sdl3
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengles2.h>
// libc
#include <err.h>
#include <stdio.h>

#define SDL_WINDOW_WIDTH 1280
#define SDL_WINDOW_HEIGHT 720

struct appstate
{
	SDL_Window* window;
	SDL_GLContext glcontext;
	GLuint shader;
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
	struct appstate* appstate = SDL_malloc(sizeof(*appstate));
	SDL_zerop(appstate);
	return appstate;
}

void appstate_free(struct appstate* ctx)
{
	if (!ctx) return;
	SDL_DestroyWindow(ctx->window);
	SDL_GL_DestroyContext(ctx->glcontext);
	glDeleteProgram(ctx->shader);
	SDL_free(ctx);
}

static int handle_key_event([[maybe_unused]] struct appstate* ctx, SDL_Scancode key_code)
{
	switch (key_code)
	{
	case SDL_SCANCODE_ESCAPE: [[fallthrough]];
	case SDL_SCANCODE_Q: return SDL_APP_SUCCESS;
	default: break;
	}
	return SDL_APP_CONTINUE;
}

static bool compile_shader(char const* path, GLuint* shader, GLenum type)
{
	FILE* file = fopen(path, "rb");
	if (!file)
	{
		warn("fopen failed");
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
		char errbuf[512];
		glGetShaderInfoLog(frag, sizeof(errbuf), NULL, errbuf);
		fprintf(stderr, "shader compilation failed: %s\n", errbuf);
		return false;
	}
	*shader = frag;
	return true;
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv)
{
	if (argc < 1 || 3 < argc) return SDL_APP_FAILURE;

	char const* vert_file = argc >= 3 ? argv[2] : "files/base.vert";
	char const* frag_file = argc >= 2 ? argv[1] : "files/splatter.frag";

	if (!SDL_SetAppMetadata("Example SDL3 Program", "1.0.0", "fr.vokunaav.sdl3")) return SDL_APP_FAILURE;

	const struct
	{
		char const* key;
		char const* value;
	} extended_metadata[] = {
		{ SDL_PROP_APP_METADATA_CREATOR_STRING,	"Maxence MICHOT"				 },
		{ SDL_PROP_APP_METADATA_COPYRIGHT_STRING, "Placed under the MIT license" },
		{ SDL_PROP_APP_METADATA_TYPE_STRING,		 "game"							},
	};

	if (!SDL_Init(SDL_INIT_VIDEO)) return SDL_APP_FAILURE;

	for (size_t i = 0; i < SDL_arraysize(extended_metadata); ++i)
		if (!SDL_SetAppMetadataProperty(extended_metadata[i].key, extended_metadata[i].value)) return SDL_APP_FAILURE;

	if (!(*appstate = appstate_init())) return SDL_APP_FAILURE;
	struct appstate* ctx = *appstate;

	ctx->window_width = SDL_WINDOW_WIDTH;
	ctx->window_height = SDL_WINDOW_HEIGHT;

	if (!(ctx->window = SDL_CreateWindow("Example SDL3 Program", ctx->window_width, ctx->window_height,
										 SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL)))
		return SDL_APP_FAILURE;

	if (!(ctx->glcontext = SDL_GL_CreateContext(ctx->window))) return SDL_APP_FAILURE;

	GLuint vert;
	if (!compile_shader(vert_file, &vert, GL_VERTEX_SHADER)) return SDL_APP_FAILURE;

	GLuint frag;
	if (!compile_shader(frag_file, &frag, GL_FRAGMENT_SHADER)) return SDL_APP_FAILURE;

	ctx->shader = glCreateProgram();
	glAttachShader(ctx->shader, vert);
	glAttachShader(ctx->shader, frag);
	glLinkProgram(ctx->shader);
	glDeleteShader(vert);
	glDeleteShader(frag);
	GLint success;
	glGetProgramiv(ctx->shader, GL_LINK_STATUS, &success);
	if (!success)
	{
		char errbuf[512];
		glGetProgramInfoLog(ctx->shader, sizeof(errbuf), NULL, errbuf);
		fprintf(stderr, "shader program failed: %s\n", errbuf);
		return SDL_APP_FAILURE;
	}
	glUseProgram(ctx->shader);

	GLfloat const vertices[] = {
		1.0, -1.0, 0.0, -1.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, -1.0, 0.0,
	};

	GLushort const index[] = { 0, 1, 2, 3 };

	GLuint v_buff;
	glGenBuffers(1, &v_buff);
	glBindBuffer(GL_ARRAY_BUFFER, v_buff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint i_buff;
	glGenBuffers(1, &i_buff);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buff);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

	GLint position = glGetAttribLocation(ctx->shader, "position");
	glVertexAttribPointer(position, 2, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(position);

	glViewport(0, 0, ctx->window_width, ctx->window_height);
	GLint u_resolution = glGetUniformLocation(ctx->shader, "u_resolution");
	glUniform2f(u_resolution, ctx->window_width, ctx->window_height);

	glClearColor(0, 0, 0, 1);

	ctx->last_step = SDL_GetTicks();
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
	struct appstate* ctx = appstate;
	glClear(GL_COLOR_BUFFER_BIT);
	GLint u_time = glGetUniformLocation(ctx->shader, "u_time");
	glUniform1f(u_time, SDL_GetTicks() / 1000.f);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, 0);
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
		glViewport(0, 0, ctx->window_width, ctx->window_height);
		GLint u_resolution = glGetUniformLocation(ctx->shader, "u_resolution");
		glUniform2f(u_resolution, ctx->window_width, ctx->window_height);
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

