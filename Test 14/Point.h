#ifndef POINT_H
#define POINT_H

#include<math.h>
#include<vector>
#include <random>
#include <chrono>

using namespace std;

int getRand(int l = 0, int r = 1000000007);

class Point
{
	public:

		float x, y;

		Point(float _x = 0, float _y = 0);
		~Point();

		//	Sum
		Point operator + (Point a);
		
		//	Subtraction
		Point operator - (Point a);

		//	Cross product
		float operator * (Point a);

		//	Dot product
		float operator % (Point a);

		void rotate(float theta);
		float angle();
		bool insideRectangle(vector<Point> rectangle);
};

#endif