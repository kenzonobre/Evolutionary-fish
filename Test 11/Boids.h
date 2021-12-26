#ifndef BOIDS_H
#define BOIDS_H

#include "Fish.h"

class Boids
{
	public : 

		vector<Fish> fish;
		int8 r, g, b;	
		int8 alignmentWeigh, cohesionWeigh, separationWeigh;

		Boids(vector<Fish> _fish = vector<Fish>{}, 
			int8 _r = 0, int8 _g = 0, int8 _b = 0,
			 int8 _a = 0, int8 _c = 0, int8 _s = 0);
		~Boids();

		void createBoids(int boidsSize);

		void moveBoids(vector<Fish> sharks);

		void drawBoids();
};

#endif