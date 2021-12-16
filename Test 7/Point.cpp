#include"Point.h"

Point::Point(double _x, double _y)
{
	this->x = _x;
	this->y = _y;
}

Point Point::operator + (Point a)
{
	return Point(this->x + a.x, this->y + a.y);
}

Point Point::operator - (Point a)
{
	return Point(this->x - a.x, this->y - a.y);
}

double Point::operator * (Point a)
{
	return this->x * a.y - this->y * a.x;
}

double Point::operator % (Point a)
{
	return this->x * a.x + this->y * a.y;
}

void Point::rotate(double theta)
{
	double nx = this->x * cos(theta) - this->y * sin(theta);
	double ny = this->x * sin(theta) + this->y * cos(theta);
	this->x = nx;
	this->y = ny;
}

bool Point::insideRectangle(vector<Point> rectangle)
{	
	for(int i = 0; i < 4; i++)
		if((rectangle[(i + 1) % 4] - rectangle[i]) * ((*this) - rectangle[i]) < 0)
			return false;

	return true;
}
