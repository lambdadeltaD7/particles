#include "world.h"
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <random>

char buffer[256];	

void render_text(
    SDL_Renderer* renderer,
    TTF_Font* font,
    const char* text,
    float x,
    float y
)
{
    SDL_Color color = {255, 255, 255, 255};

    SDL_Surface* surface =
        TTF_RenderText_Blended(font, text, 0, color);

    SDL_Texture* texture =
        SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FRect rect = {
        x,
        y,
        (float)surface->w,
        (float)surface->h
    };

    SDL_RenderTexture(renderer, texture, NULL, &rect);

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}

std::unordered_map<int, Particle> spawn_particles(
	std::mt19937_64& rgen
)
{
	int next_particle_id = 0;
	std::unordered_map<int, Particle> particles;
	std::uniform_real_distribution<double> dist(-MAX_INIT_SPEED, MAX_INIT_SPEED);

	for(int i=0; i<CNT_INIT_PARTICLES; ++i)
	{
		Particle p;
		p.pos.x = rgen() % WINDOW_WIDTH;
		p.pos.y = rgen() % WINDOW_HEIGHT;

		p.v.x = dist(rgen);
		p.v.y = dist(rgen);

		p.m = 1 + rgen() % MAX_MASS;
		p.m = std::max((float)MIN_MASS, p.m);
		particles[next_particle_id++] = p;
	}

	return particles;
}

Vec is_out(const Particle& p){
	if(p.pos.x < -WORLD_WIDTH) return Vec(1,0);
	if(p.pos.y < -WORLD_HEIGHT) return Vec(0,1);
	if(p.pos.x > WORLD_WIDTH) return Vec(-1,0);
	if(p.pos.y > WORLD_HEIGHT) return Vec(0,-1);
	return Vec(0,0);
}


void handle_reflection(Particle& p)
{
	Vec n = is_out(p);
	if(n.x==0 && n.y==0)
		return;

	// time travel or smthn idk
	p.pos -= p.v * TIME_DELTA_SEC;
	
	float dot = p.v.x * n.x + p.v.y * n.y;

    p.v.x -= 2 * dot * n.x;
    p.v.y -= 2 * dot * n.y;
}


void update_particles_state(
	std::unordered_map<int, Particle>& particles
)
{
	// update forces
	std::unordered_map<int, Vec> Fs;
	for(auto& [id1,p1] : particles)
	{
		Fs[id1] = Vec(0,0);
		for(auto it=particles.begin(); it!=particles.end(); ++it)
		{
			auto& [id2,p2] = *it;
			if(id1 == id2)
				continue;

			float dx = p2.pos.x - p1.pos.x;
			float dy = p2.pos.y - p1.pos.y;
			float d = sqrt(dx*dx + dy*dy);

			float F_abs = GRAVITATIONAL_CONSTANT * p1.m * p2.m / (d * d + NONZERO);
			Vec F_direction(dx/d, dy/d);
			Fs[id1] += (F_abs * F_direction);
		}
	}

	// update velocity
	for(auto& [id1,p1] : particles)
	{
		Vec a = Fs[id1] * (1 / p1.m);
		p1.v += a * TIME_DELTA_SEC;
	}
	
	// update positions
	for(auto& [id,p] : particles)
	{
		p.pos += p.v * TIME_DELTA_SEC;
		handle_reflection(p);
	}
	
	// handle collisions
	for(auto& [id1,p1] : particles)
	{
		for(auto it=particles.begin(); it!=particles.end(); )
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
			
			if((d < std::max(p1.m, p2.m)) && (p1.m+p2.m < 200))
			{
				float inv = (1 / (p1.m + p2.m)); 
				p1.v = inv * (p1.m * p1.v + p2.m * p2.v);
				p1.pos = inv * (p1.m * p1.pos + p2.m * p2.pos);
				// printf("mrg %d to %d m2=%.3f, m1=%.3f\n", id2, id1, p2.m, p1.m);
				p1.m += p2.m;
				// printf("new_m=%.3f\n\n", p1.m);
				it = particles.erase(it);
				continue;
			}

			++it;
		}
	}
	

}

void render_all(
	SDL_Renderer *renderer,
	const std::unordered_map<int, Particle>& particles,
	const DisplayConfig& dc,
	TTF_Font* font
)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	SDL_FRect re;
	re.x=0;
	re.y=0;
	re.w=WINDOW_WIDTH;
	re.h=WINDOW_HEIGHT;
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderRect(renderer, &re);

	render_all_particles(renderer, particles, dc);

	sprintf(buffer, "(x0,y0)=(%.2f,%.2f) | scale=%.2f | (w,h)=(%.2f,%.2f) | particles=%li", dc.x0, dc.y0, dc.scale, WINDOW_WIDTH / dc.scale, WINDOW_HEIGHT / dc.scale, particles.size());
	render_text(renderer, font, buffer, 10.0, 0.0);

	SDL_RenderPresent(renderer);
}

void render_all_particles(
	SDL_Renderer *renderer,
	const std::unordered_map<int, Particle>& particles,
	const DisplayConfig& dc
)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

	for(const auto& [id,p] : particles){
		render_particle(renderer, p, dc);
	} 
}

Vec map_cords(float x, float y, DisplayConfig dc)
{
	float x_display = (x - dc.x0) * dc.scale + (float)WINDOW_WIDTH / 2;
	float y_display = (y - dc.y0) * dc.scale + (float)WINDOW_HEIGHT / 2;
	return Vec(x_display, y_display);
}
	
void render_particle(
	SDL_Renderer *renderer,
	const Particle& p,
	const DisplayConfig& dc
)
{
	SDL_FRect rect;
	Vec disp_cords = map_cords(
				(p.pos.x - p.m - 1),
				(p.pos.y - p.m - 1),
				dc
			);
	rect.x = disp_cords.x;
	rect.y = disp_cords.y;
	rect.w = (2 * p.m + 1) * dc.scale;
	rect.h = (2 * p.m + 1) * dc.scale;

	SDL_RenderFillRect(renderer, &rect);
}

void print_dc(DisplayConfig dc)
{
	printf("(x0, y0) = (%.2f, %.2f)\n", dc.x0, dc.y0);
	printf("(w, h) = (%.2f, %.2f) \n", (float)WINDOW_WIDTH * dc.scale, WINDOW_HEIGHT * dc.scale);
	printf("scale = %.3f\n\n", dc.scale);
}

