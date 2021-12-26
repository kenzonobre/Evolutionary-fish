#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

#ifndef FISH_H
#define FISH_H

#include "Point.h"

#define fishSize 0.02
#define sharkSize 0.05
#define fishSpeed 10
#define sharkSpeed 10
#define fishVision 10
#define avoidSharkWeigh 100

typedef unsigned char int8;

class Fish
{
	public : 

		Point coord;
		double dx, dy;
		bool isShark;

		Fish(double _x = 0, double _y = 0, double _dx = 0, double _dy = 0, bool _isShark = false);
		~Fish();

		double getSpeed();
		double getDistance(Fish other);

		void move();
		void alignment(vector<Fish> neighbors, int8 alignmentWeigh);
		void cohesion(vector<Fish> neighbors, int8 cohesionWeigh);
		void separation(vector<Fish> neighbors, int8 separationWeigh);

		void matchSpeed(vector<Fish> neighbors);
		void limitSpeed();
		void avoidSharks(vector<Fish> sharks);
		void chaseFish(vector<Fish> neighbors);
		bool eatenByShark(vector<Fish> sharks);

		void drawFish(int8 r, int8 g, int8 b);
		void drawShark();
};

#endif