#ifndef FISH_H
#define FISH_H

#include "Point.h"

#define FISH_SIZE 0.02
#define FISH_MAX_SPEED 15

#define SHARK_SIZE 0.05
#define SHARK_MAX_SPEED 15

#define FISH_DISTANCE_VISION 10
#define SHARK_DISTANCE_VISION 5

#define	ANGLE_OF_VISION 300

class Fish
{
	public : 

		Point coord;
		float dx, dy;
		bool isShark;

		//	Fish Constructor
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