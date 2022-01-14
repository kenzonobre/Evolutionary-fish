#ifndef BOID_H
#define BOID_H

#include "Fish.h"

#define BOIDS_SIZE 20

class Boid
{
	public : 

		vector<Fish> fish;
		int8 r, g, b;	
		int8 alignmentWeigh, cohesionWeigh, separationWeigh, avoidSharkWeigh;

		//	Boid Constructor
		Boid(vector<Fish> _fish = vector<Fish>{}, 
			int8 _r = 0, int8 _g = 0, int8 _b = 0,
			 int8 _a = 0, int8 _c = 0, int8 _s = 0, int8 _av = 0);
		~Boid();

		//	Crossover between two Boids
		Boid operator + (Boid other);
		
		//	Mutation
		void mutateBoid(int taxOfMutation);

		void moveBoid(vector<Fish> sharks);
		void drawBoid();
};

Boid generateRandomBoid();

#endif