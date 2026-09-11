#include "world.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>

std::unordered_map<int, Particle> spawn_particles(
	std::mt19937_64& rgen
)
{
	int next_particle_id = 0;
	std::unordered_map<int, Particle> particles;

	for(int i=0; i<CNT_INIT_PARTICLES; ++i)
	{
		Particle p;
		p.pos.x = rgen() % WINDOW_WIDTH;
		p.pos.y = rgen() % WINDOW_HEIGHT;
		// what about speed init?
		p.m = MIN_MASS + rgen() % MAX_MASS;
		particles[next_particle_id++] = p;
	}

	return particles;
}

bool is_out(const Particle& p){
	return p.pos.x < 0 || 
		   p.pos.x > WINDOW_WIDTH ||
		   p.pos.y < 0 ||
		   p.pos.y > WINDOW_HEIGHT;
}

void update_particles_state(
	std::unordered_map<int, Particle>& particles
)
{
	for(auto& [id1,p1] : particles)
	{
		Vec F_total(0,0);

		for(auto& [id2,p2] : particles)
		{
			float dx = p2.pos.x - p1.pos.x;
			float dy = p2.pos.y - p1.pos.y;
			float d = sqrt(dx*dx + dy*dy);
			if(d < DIST_EPS){
				// implement merge
				continue;
			}
			float F_abs = GRAVITATIONAL_CONSTANT * p1.m * p2.m / (d * d);
			Vec F_direction(dx/d, dy/d);
			F_total += (F_abs * F_direction);
		}

		Vec a = F_total * (1 / p1.m);
		p1.v += a * TIME_DELTA_SEC;
	}


	for(auto& [id,p] : particles)
	{
		p.pos += p.v * TIME_DELTA_SEC;

		if(is_out(p))
			p.v *= (-1);
	}
}

void render_all(
	SDL_Renderer *renderer,
	const std::unordered_map<int, Particle>& particles
)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	render_all_particles(renderer, particles);

	SDL_RenderPresent(renderer);
}

void render_all_particles(
	SDL_Renderer *renderer,
	const std::unordered_map<int, Particle>& particles
)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

	for(const auto& [id,p] : particles){
		render_particle(renderer, p);
	} 
}

void render_particle(
	SDL_Renderer *renderer,
	const Particle& p
)
{
	SDL_FRect rect;
	rect.x = p.pos.x - p.m - 1;
	rect.y = p.pos.y - p.m - 1;
	rect.w = 2 * p.m + 1;
	rect.h = 2 * p.m + 1;

	SDL_RenderFillRect(renderer, &rect);
}
