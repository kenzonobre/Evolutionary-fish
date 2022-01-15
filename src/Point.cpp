#include"Point.h"

Point::Point(float _x, float _y)
{
	this->x = _x;
	this->y = _y;
}

Point::~Point()
{
	return;
}

//	Sum of vectors
Point Point::operator + (Point other)
{
	return Point(this->x + other.x, this->y + other.y);
}

//	Subtraction of vectors
Point Point::operator - (Point other)
{
	return Point(this->x - other.x, this->y - other.y);
}

//	Cross product
float Point::operator * (Point other)
{
	return this->x * other.y - this->y * other.x;
}

//	Dot product
float Point::operator % (Point other)
{
	return this->x * other.x + this->y * other.y;
}

//	Rotate the vector by theta radians
void Point::rotate(float theta)
{
	float nx = this->x * cos(theta) - this->y * sin(theta);
	float ny = this->x * sin(theta) + this->y * cos(theta);
	this->x = nx;
	this->y = ny;
}

//	Get angle of the vector
float Point::angle()
{
	float theta = atan2(this->y, this->x);
	if(theta < 0)	theta += 2 * M_PI;

	return theta;
}

//	Check if the point is inside a rectangle 
bool Point::insideRectangle(vector<Point> rectangle)
{	
	for(int i = 0; i < 4; i++)
		if((rectangle[(i + 1) % 4] - rectangle[i]) * ((*this) - rectangle[i]) < 0)
			return false;

	return true;
}
