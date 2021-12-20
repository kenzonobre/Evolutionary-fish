#include "Boids.h"

Boids::Boids(vector<Fish> _fish, int8 _r, int8 _g, int8 _b, int8 _c, int8 _a, int8 _s)
{
	fish = _fish;
	r = _r;
	g = _g;
	b = _b;
	cohesionWeigh = _c;
	alignmentWeigh = _a;
	separationWeigh = _s;
}

Boids::~Boids()
{
	return;
}

void Boids::drawBoids()
{
	for(int i = 0; i < (int) fish.size(); i++)
		fish[i].drawFish(r, g, b);
}