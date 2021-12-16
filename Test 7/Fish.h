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

class Fish
{
	public : 

		Point coord = Point(0, 0);
		double dx, dy;
		unsigned char r, g, b;
		bool isShark;

		Fish(double _x, double _y, double _dx, double _dy, bool _isShark, 
			unsigned char _r, unsigned char _g, unsigned char _b);

		double getSpeed();

		double getDistance(Fish other);

		void move();

		void drawFish();

		void drawShark();
};

#endif