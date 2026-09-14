#include <SDL3/SDL.h>
#include <SDL3/SDL_oldnames.h>
#include <iostream>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include "world.h"


int main()
{
	DisplayConfig dc;

	bool fullscreen = false;
	SDL_WindowFlags flags = 
		fullscreen & SDL_WINDOW_FULLSCREEN |
		SDL_WINDOW_RESIZABLE;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	SDL_Init(SDL_INIT_VIDEO);

	SDL_CreateWindowAndRenderer("title here", WINDOW_WIDTH, WINDOW_HEIGHT, flags,  &window, &renderer);
	SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

	bool running = true;
	SDL_Event event;

	std::mt19937_64 rgen(RANDOM_SEED);
	auto particles = spawn_particles(rgen);
// 	particles[0].m=60;
// 	particles[0].pos = Vec(200,200);
// 	particles[1].m=60;
// 	particles[1].pos = Vec(900,200);
// 	particles[2].m=2;
// 	particles[2].pos = Vec(200,300);
// 	particles[2].v = Vec(1e-6,1e-7);
// 	particles[3].m=2;
// 	particles[3].pos = Vec(900,300);
// 	particles[3].v = Vec(-1e-6, 1e-7);

	update_particles_state(particles);
	render_all(renderer, particles, dc);
	while(running)
	{
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_EVENT_QUIT)
				running = false;
			if(event.type  == SDL_EVENT_KEY_DOWN)
			{
				if(event.key.key == SDLK_Q)
					dc.scale = std::max(dc.scale - 0.1, 0.1);
				else if(event.key.key == SDLK_E)
					dc.scale = std::min(dc.scale + 0.1, 5.0);
				else if(event.key.key == SDLK_RIGHT)
					dc.x0 += dc.scale * WINDOW_WIDTH * 0.1;
				else if(event.key.key == SDLK_LEFT)
					dc.x0 -= dc.scale * WINDOW_WIDTH * 0.1;
				else if(event.key.key == SDLK_DOWN)
					dc.y0 += dc.scale * WINDOW_HEIGHT * 0.1;
				else if(event.key.key == SDLK_UP)
					dc.y0 -= dc.scale * WINDOW_HEIGHT * 0.1;
				print_dc(dc);	
			}
		}

		update_particles_state(particles);
		render_all(renderer, particles, dc);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
}
