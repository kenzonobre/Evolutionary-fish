#include"Point.h"

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

int getRand(int l, int r)
{
	return uniform_int_distribution<int>(l, r)(rng);
}

Point::Point(float _x, float _y)
{
	this->x = _x;
	this->y = _y;
}

Point::~Point()
{
	return;
}

Point Point::operator + (Point a)
{
	return Point(this->x + a.x, this->y + a.y);
}

Point Point::operator - (Point a)
{
	return Point(this->x - a.x, this->y - a.y);
}

float Point::operator * (Point a)
{
	return this->x * a.y - this->y * a.x;
}

float Point::operator % (Point a)
{
	return this->x * a.x + this->y * a.y;
}

void Point::rotate(float theta)
{
	float nx = this->x * cos(theta) - this->y * sin(theta);
	float ny = this->x * sin(theta) + this->y * cos(theta);
	this->x = nx;
	this->y = ny;
}

float Point::angle()
{
	float theta = atan2(y, x);
	if(theta < 0)	 
		theta += 2 * M_PI;

	return theta;
}

bool Point::insideRectangle(vector<Point> rectangle)
{	
	for(int i = 0; i < 4; i++)
		if((rectangle[(i + 1) % 4] - rectangle[i]) * ((*this) - rectangle[i]) < 0)
			return false;

	return true;
}
