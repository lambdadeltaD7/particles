#include <SDL3/SDL.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include "world.h"


void handle_key(
	const SDL_Event& event,
	DisplayConfig& dc
)
{
	if(event.key.key == SDLK_Q)
		dc.scale = std::max(dc.scale - 0.1, 0.1);

	else if(event.key.key == SDLK_E)
		dc.scale = std::min(dc.scale + 0.1, 5.0);

	else if(event.key.key == SDLK_RIGHT)
		dc.x0 +=  WINDOW_WIDTH * 0.1 / dc.scale;

	else if(event.key.key == SDLK_LEFT)
		dc.x0 -=  WINDOW_WIDTH * 0.1 / dc.scale;

	else if(event.key.key == SDLK_DOWN)
		dc.y0 +=  WINDOW_HEIGHT * 0.1 / dc.scale;

	else if(event.key.key == SDLK_UP)
		dc.y0 -=  WINDOW_HEIGHT * 0.1 / dc.scale;

	else if(event.key.key == SDLK_C)
		dc = DisplayConfig{};
}

int main()
{
	DisplayConfig dc;

	bool fullscreen = true;
	SDL_WindowFlags flags = 
		fullscreen & SDL_WINDOW_FULLSCREEN |
		SDL_WINDOW_RESIZABLE;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	TTF_Font* font = TTF_OpenFont(
    "/usr/share/fonts/TTF/JetBrainsMonoNerdFont-Regular.ttf",
    12
);

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

	while(running)
	{
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_EVENT_QUIT)
				running = false;
			if(event.type  == SDL_EVENT_KEY_DOWN)
				handle_key(event, dc);
		}

		update_particles_state(particles);
		render_all(renderer, particles, dc, font);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
}
