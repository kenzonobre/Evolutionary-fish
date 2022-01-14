#include "Boid.h"

//	Boid Constructor
Boid::Boid(vector<Fish> _fish, int8 _r, int8 _g, int8 _b, int8 _a, int8 _c, int8 _s, int8 _av)
{
	this->fish = _fish;
	this->r = _r;
	this->g = _g;
	this->b = _b;
	this->alignmentWeigh = _a;
	this->cohesionWeigh = _c;
	this->separationWeigh = _s;
	this->avoidSharkWeigh = _av;
}

Boid::~Boid()
{
	return;
}

//	Crossover between two Boids
Boid Boid::operator + (Boid other)
{
	Boid newBoid;
	newBoid = generateRandomBoid();

	newBoid.r = (this->r + other.r) / 2;
	newBoid.g = (this->g + other.g) / 2;
	newBoid.b = (this->b + other.b) / 2;

	newBoid.alignmentWeigh = (this->alignmentWeigh + other.alignmentWeigh) / 2;
	newBoid.cohesionWeigh = (this->cohesionWeigh + other.cohesionWeigh) / 2;
	newBoid.separationWeigh = (this->separationWeigh + other.separationWeigh) / 2;
	newBoid.avoidSharkWeigh = (this->avoidSharkWeigh + other.avoidSharkWeigh) / 2;

	return newBoid;
}

//	Mutation
void Boid::mutateBoid(int taxOfMutation)
{
	this->r += ((getRand(0, 1)) ? 1 : -1) * taxOfMutation * 10;
	this->g += ((getRand(0, 1)) ? 1 : -1) * taxOfMutation * 10;
	this->b += ((getRand(0, 1)) ? 1 : -1) * taxOfMutation * 10;
	this->alignmentWeigh += ((getRand(0, 1)) ? 1 : -1) * taxOfMutation;
	this->cohesionWeigh += ((getRand(0, 1)) ? 1 : -1) * taxOfMutation;
	this->separationWeigh += ((getRand(0, 1)) ? 1 : -1) * taxOfMutation;
	this->avoidSharkWeigh += ((getRand(0, 1)) ? 1 : -1) * taxOfMutation;

	limitRange(this->r, 0, 255);
	limitRange(this->g, 0, 255);
	limitRange(this->b, 0, 255);
	limitRange(this->alignmentWeigh, 5, 100);
	limitRange(this->cohesionWeigh, 5, 100);
	limitRange(this->separationWeigh, 5, 100);
	limitRange(this->avoidSharkWeigh, 5, 100);
}

void Boid::moveBoid(vector<Fish> sharks)
{
	for(int i = 0; i < (int) this->fish.size(); i++)
	{
		this->fish[i].alignment(fish, alignmentWeigh);
		this->fish[i].cohesion(fish, cohesionWeigh);
		this->fish[i].separation(fish, separationWeigh);
		this->fish[i].avoidSharks(sharks, avoidSharkWeigh);
		this->fish[i].limitSpeed();
		this->fish[i].move();
	}
}

void Boid::drawBoid()
{
	for(int i = 0; i < (int) this->fish.size(); i++)
		this->fish[i].drawFish(r, g, b);
}

Boid generateRandomBoid()
{
	vector<Fish> fish(BOIDS_SIZE);
	fish[0] = generateRandomFish(false);
	for(int i = 1; i < BOIDS_SIZE; i++)
		fish[i] = fish[0];

	float r = 0, g = 0, b = 0;
	while(r + g + b < 150 || 400 < r + g + b)
	{
		r = getRand() % 256;
		g = getRand() % 256;
		b = getRand() % 256;
	}

	int8 alignmentWeigh = getRand() % 96 + 5;
	int8 cohesionWeigh = getRand() % 96 + 5;
	int8 separationWeigh = getRand() % 96 + 5;
	int8 avoidSharkWeigh = getRand() % 96 + 5;

	return Boid(fish, r, g, b, alignmentWeigh, cohesionWeigh, separationWeigh, avoidSharkWeigh);
}