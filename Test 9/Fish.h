#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

#ifndef FISH_H
#define FISH_H

#include<math.h>
#include "Point.h"

#define fishSize 0.02
#define sharkSize 0.05
#define fishSpeed 10
#define sharkSpeed 10
#define fishVision 7

typedef unsigned char int8;

class Fish
{
	public : 

		Point coord;
		double dx, dy;
		bool isShark;

		Fish(double _x = 0, double _y = 0, double _dx = 0, double _dy = 0, bool _isShark = false);

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

		void drawFish(int8 r, int8 g, int8 b);
		void drawShark();
};

#endif