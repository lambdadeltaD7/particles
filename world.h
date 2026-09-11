#include "vec.h"
#include <random>
#include <SDL3/SDL.h>
#include <SDL3/SDL_pixels.h>
#include <random>
#include <unordered_map>


const int WINDOW_WIDTH  = 800;
const int WINDOW_HEIGHT = 800;
const int CNT_INIT_PARTICLES = 64;
const int RANDOM_SEED = 67;
const int MAX_MASS = 30;
const int MIN_MASS = 1;
const float GRAVITATIONAL_CONSTANT = 6.67e-3;
const float TIME_DELTA_SEC = 1.1; 


struct Particle
{
	Vec pos;
	Vec v;
	float m = 0;
};


std::unordered_map<int, Particle> spawn_particles(
	std::mt19937_64& rgen
);

void render_all(
	SDL_Renderer *renderer,
	const std::unordered_map<int, Particle>& particles
);

void render_all_particles(
	SDL_Renderer *renderer,
	const std::unordered_map<int, Particle>& particles
);

void update_particles_state(
	std::unordered_map<int, Particle>& particles
);

void render_particle(
	SDL_Renderer *renderer,
	const Particle& p
);
