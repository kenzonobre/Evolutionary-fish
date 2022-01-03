#include "Boid.h"

Boid::Boid(vector<Fish> _fish, int8 _r, int8 _g, int8 _b, int8 _a, int8 _c, int8 _s, int8 _av)
{
	fish = _fish;
	r = _r;
	g = _g;
	b = _b;
	alignmentWeigh = _a;
	cohesionWeigh = _c;
	separationWeigh = _s;
	avoidSharkWeigh = _av;
}

Boid::~Boid()
{
	return;
}

void Boid::moveBoid(vector<Fish> sharks)
{
	for(int i = 0; i < (int) fish.size(); i++)
	{
		fish[i].alignment(fish, alignmentWeigh);
		fish[i].cohesion(fish, cohesionWeigh);
		fish[i].separation(fish, separationWeigh);
		fish[i].avoidSharks(sharks, avoidSharkWeigh);
		fish[i].limitSpeed();
		fish[i].move();
	}
}

void Boid::drawBoid()
{
	for(int i = 0; i < (int) fish.size(); i++)
		fish[i].drawFish(r, g, b);
}

Boid generateRandomBoid(int boidSize)
{
	vector<Fish> fish(boidSize);
	fish[0] = generateRandomFish(false);
	for(int i = 1; i < boidSize; i++)
		fish[i] = fish[0];

	float r = 0, g = 0, b = 0;
	while(r + g + b < 150 || 400 < r + g + b)
	{
		r = getRand() % 256;
		g = getRand() % 256;
		b = getRand() % 256;
	}

	int8 alignmentWeigh = getRand() % 100;
	int8 cohesionWeigh = getRand() % 100;
	int8 separationWeigh = getRand() % 100;
	int8 avoidSharkWeigh = getRand() % 100;

	return Boid(fish, r, g, b, alignmentWeigh, cohesionWeigh, separationWeigh, avoidSharkWeigh);
}