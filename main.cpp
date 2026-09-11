#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include "world.h"

int main()
{
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	SDL_Init(SDL_INIT_VIDEO);

	SDL_CreateWindowAndRenderer("title here", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer);
	SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

	bool running = true;
	SDL_Event event;

	std::mt19937_64 rgen(RANDOM_SEED);
	auto particles = spawn_particles(rgen);

	while(running)
	{
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_EVENT_QUIT)
				running = false;
		}

		update_particles_state(particles);
		render_all(renderer, particles);

	}

	SDL_DestroyWindow(window);
	SDL_Quit();
}
