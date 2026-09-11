#include "world.h"
#include <iostream>
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

Vec is_out(const Particle& p){
	if(p.pos.x < 0) return Vec(0,1);
	if(p.pos.y < 0) return Vec(1,0);
	if(p.pos.x > WINDOW_WIDTH) return Vec(0,-1);
	if(p.pos.y > WINDOW_HEIGHT) return Vec(-1,0);
	return Vec(0,0);
}

float norm(const Vec& v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

void handle_reflection(Particle& p)
{
	Vec res = is_out(p);
	if(res.x==0 && res.y==0)
	{
		return;
	}

	printf("bord_vec: (%f.3f, %f.3f)\n", res.x, res.y);

	// time travel or smthn idk
	p.pos -= p.v * TIME_DELTA_SEC;
	
	float dot = p.v.x * res.x + p.v.y * res.y; 
	float v_norm = norm(p.v);
	// div by zero?
	float v_cos = dot / v_norm;
	float v_sin = sqrt(1 - v_cos * v_cos);
	printf("c=%f.3f s=%f.3f\n", v_cos, v_sin);
	printf("d=%f.3f\n", v_cos*v_cos + v_sin*v_sin);
	printf("n1=%f.3f\n", v_norm);
	if(v_cos <= 0)
	{
		v_sin *= -1;
	}

	p.v.x = v_cos * p.v.x - v_sin * p.v.y; 
	p.v.y = v_sin * p.v.x + v_cos * p.v.y; 
	printf("n2=%f.3f\n\n", norm(p.v));

	std::cout << std::flush;
}

void update_particles_state(
	std::unordered_map<int, Particle>& particles
)
{
	for(auto& [id1,p1] : particles)
	{
		Vec F_total(0,0);

		for(auto it=particles.begin(); it!=particles.end();)
		{
			auto& [id2,p2] = *it;
			if(id1 == id2)
			{
				++it;
				continue;
			}

			float dx = p2.pos.x - p1.pos.x;
			float dy = p2.pos.y - p1.pos.y;
			float d = sqrt(dx*dx + dy*dy);
			// merge p2 into p1 if they are close
			if(d < DIST_EPS){
				// rework according to radius
				// printf("mrg %d into %d\n", id2, id1);
				// std::cout << std::flush;
				p1.m += p2.m;
				it = particles.erase(it);
				continue;
			}
			float F_abs = GRAVITATIONAL_CONSTANT * p1.m * p2.m / (d * d);
			Vec F_direction(dx/d, dy/d);
			F_total += (F_abs * F_direction);

			++it;
		}

		Vec a = F_total * (1 / p1.m);
		p1.v += a * TIME_DELTA_SEC;
	}


	for(auto& [id,p] : particles)
	{
		p.pos += p.v * TIME_DELTA_SEC;
		handle_reflection(p);
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
