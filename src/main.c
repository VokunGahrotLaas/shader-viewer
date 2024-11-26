// sdl3
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define SDL_WINDOW_WIDTH 1'280
#define SDL_WINDOW_HEIGHT 720

struct appstate
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	uint64_t last_step;
};

struct appstate* appstate_init(void);
void appstate_free(struct appstate* ctx);

static int handle_key_event(struct appstate* ctx, SDL_Scancode key_code);

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv);
SDL_AppResult SDL_AppIterate(void* appstate);
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event);
void SDL_AppQuit(void* appstate, SDL_AppResult result);

struct appstate* appstate_init(void)
{
	struct appstate* appstate = SDL_malloc(sizeof(*appstate));
	return appstate;
}

void appstate_free(struct appstate* ctx)
{
	if (!ctx) return;
	SDL_DestroyRenderer(ctx->renderer);
	SDL_DestroyWindow(ctx->window);
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

SDL_AppResult SDL_AppInit(void** appstate, [[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
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

	if (!SDL_CreateWindowAndRenderer("Example SDL3 Program", SDL_WINDOW_WIDTH, SDL_WINDOW_HEIGHT, SDL_WINDOW_BORDERLESS,
									 &ctx->window, &ctx->renderer))
		return SDL_APP_FAILURE;

	ctx->last_step = SDL_GetTicks();
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
	struct appstate* ctx = appstate;
	SDL_SetRenderDrawColor(ctx->renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(ctx->renderer);
	SDL_RenderPresent(ctx->renderer);
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	struct appstate* ctx = appstate;
	switch (event->type)
	{
	case SDL_EVENT_QUIT: return SDL_APP_SUCCESS;
	case SDL_EVENT_KEY_DOWN: return handle_key_event(ctx, event->key.scancode);
	default: break;
	}
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, [[maybe_unused]] SDL_AppResult result)
{
	struct appstate* ctx = appstate;
	appstate_free(ctx);
}

