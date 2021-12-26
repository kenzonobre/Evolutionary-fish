#include "Boids.h"
#include <time.h>
#include<random>

Boids::Boids(vector<Fish> _fish, int8 _r, int8 _g, int8 _b, int8 _a, int8 _c, int8 _s)
{
	fish = _fish;
	r = _r;
	g = _g;
	b = _b;
	alignmentWeigh = _a;
	cohesionWeigh = _c;
	separationWeigh = _s;
}

Boids::~Boids()
{
	return;
}

void Boids::createBoids(int boidsSize)
{
	float x = (float) (rand() % 1000) / 1000;
	float y = (float) (rand() % 1000) / 1000;
	float dx = (float) (rand() % fishSpeed) / 1000;
	float dy = (float) (rand() % fishSpeed) / 1000;

	if(rand() & 1)	dx *= -1;
	if(rand() & 1)	dy *= -1;

	while(r + g + b < 150 || 400 < r + g + b)
	{
		r = rand() % 256;
		g = rand() % 256;
		b = rand() % 256;
	}

	//if(rand() & 1)	x *= -1;
	if(rand() & 1)	y *= -1;

	fish.clear();
	for(int j = 0; j < boidsSize; j++)
		fish.push_back(Fish(x, y, dx, dy, false));

	alignmentWeigh = rand() % 40 + 10;
	cohesionWeigh = rand() % 40 + 10;
	separationWeigh = rand() % 40 + 10;
}

void Boids::moveBoids(vector<Fish> sharks)
{
	for(int i = 0; i < (int) fish.size(); i++)
	{
		fish[i].alignment(fish, alignmentWeigh);
		fish[i].cohesion(fish, cohesionWeigh);
		fish[i].separation(fish, separationWeigh);
		fish[i].avoidSharks(sharks);
		//fish[i].matchSpeed(fish);
		fish[i].limitSpeed();
		fish[i].move();
	}
}

void Boids::drawBoids()
{
	for(int i = 0; i < (int) fish.size(); i++)
		fish[i].drawFish(r, g, b);
}