#ifndef POINT_H
#define POINT_H

#include "MyUtility.h"

using namespace std;

class Point
{
	public:

		float x, y;

		Point(float _x = 0, float _y = 0);
		~Point();

		//	Sum of vectors
		Point operator + (Point a);
		
		//	Subtraction of vectors
		Point operator - (Point a);

		//	Cross product
		float operator * (Point a);

		//	Dot product
		float operator % (Point a);

		//	Rotate the vector by theta radians
		void rotate(float theta);

		//	Get angle of the vector
		float angle();

		//	Check if the point is inside a rectangle 
		bool insideRectangle(vector<Point> rectangle);
};

#endif
