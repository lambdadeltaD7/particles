#include "vec.h"
#include <random>
#include <SDL3/SDL.h>
#include <SDL3/SDL_pixels.h>
#include <random>
#include <unordered_map>
#include <SDL3_ttf/SDL_ttf.h>

const int WINDOW_WIDTH  = 1200;
const int WINDOW_HEIGHT = 600;
const int WORLD_WIDTH = 3'000;
const int WORLD_HEIGHT = 3'000;

const float MAX_INIT_SPEED = 1e-5;
const int CNT_INIT_PARTICLES = 1000;
const int RANDOM_SEED = 11;
const int MAX_MASS = 2;
const int MIN_MASS = 1;
const float GRAVITATIONAL_CONSTANT = 6.67e-9;
const float TIME_DELTA_SEC = 1000.1; 
const float NONZERO = 1e-5;

struct Particle
{
	Vec pos;
	Vec v;
	float m = 0;
};


struct DisplayConfig
{
	float x0 = (float)WINDOW_WIDTH / 2;
	float y0 = (float)WINDOW_HEIGHT / 2;
	float scale = 1.0;
};


void render_text(
    SDL_Renderer* renderer,
    TTF_Font* font,
    const char* text,
    float x,
    float y
);

void print_dc(DisplayConfig dc);

std::unordered_map<int, Particle> spawn_particles(
	std::mt19937_64& rgen
);

void render_world_boundary(
	SDL_Renderer *renderer,
	const DisplayConfig& dc
);

void render_all(
	SDL_Renderer *renderer,
	const std::unordered_map<int, Particle>& particles,
	const DisplayConfig& dc,
	TTF_Font* font
);

void render_all_particles(
	SDL_Renderer *renderer,
	const std::unordered_map<int, Particle>& particles,
	const DisplayConfig& dc
);

void update_particles_state(
	std::unordered_map<int, Particle>& particles
);

void render_particle(
	SDL_Renderer *renderer,
	const Particle& p,
	const DisplayConfig& dc
);
