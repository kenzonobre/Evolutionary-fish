#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

#ifndef FISH_H
#define FISH_H

#include "Point.h"

#define fishSize 0.02
#define fishSpeed 10

#define sharkSize 0.05
#define sharkSpeed 10

#define fishVision 10
#define	angleRangeVision 300
//#define avoidSharkWeigh 100

typedef unsigned char int8;

class Fish
{
	public : 

		Point coord;
		float dx, dy;
		bool isShark;

		Fish(float _x = 0, float _y = 0, float _dx = 0, float _dy = 0, bool _isShark = false);
		~Fish();

		bool insideRangeVision(Fish neighbor);
		float getSpeed();
		float getDistance(Fish neighbor);

		void move();
		void alignment(vector<Fish> neighbors, int8 alignmentWeigh);
		void cohesion(vector<Fish> neighbors, int8 cohesionWeigh);
		void separation(vector<Fish> neighbors, int8 separationWeigh);

		void limitSpeed();
		void avoidSharks(vector<Fish> sharks, int8 avoidSharkWeigh);
		void chaseFish(vector<Fish> neighbors);
		bool eatenByShark(vector<Fish> sharks);

		void drawFish(int8 r, int8 g, int8 b);
		void drawShark();
};

Fish generateRandomFish(bool isShark);

#endif