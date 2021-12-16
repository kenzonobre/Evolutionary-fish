#ifndef POINT_H
#define POINT_H

#include<math.h>
#include<vector>

using namespace std;

class Point
{
	public:

		double x, y;

		Point(double _x, double _y);

		Point operator + (Point a);

		Point operator - (Point a);

		//	Cross product
		double operator * (Point a);
		
		//	Dot product
		double operator % (Point a);

		void rotate(double theta);

		bool insideRectangle(vector<Point> rectangle);
};

#endif