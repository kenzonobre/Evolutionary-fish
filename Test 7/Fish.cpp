#include"Fish.h"
#include"Point.h"

Fish::Fish(double _x, double _y, double _dx, double _dy, bool _isShark,
	unsigned char _r, unsigned char _g, unsigned char _b)
{
	coord = Point(_x, _y);
	dx = _dx;
	dy = _dy;
	isShark = _isShark;
	r = _r;
	g = _g;
	b = _b;
}

void Fish::move()
{
	double margin = 4 * fishSize;
	double turnFactor = 0.002;

	if(isShark)
	{
		margin = 3 * sharkSize;
		turnFactor = 0.0003;
	}

	if(coord.x < -1 + margin)
		dx += turnFactor;

	if(coord.x > 1 - margin)
		dx -= turnFactor;

	if(coord.y < -1 + margin)
		dy += turnFactor;

	if(coord.y > 0.7 - margin)
		dy -= turnFactor;

	coord.x += dx;
	coord.y += dy;
}

double Fish::getSpeed()
{
	Point p(dx, dy);
	return sqrt(p % p);
}

double Fish::getDistance(Fish other)
{
	Point p = other.coord - coord;
	return sqrt(p % p);
}

void Fish::drawFish()
{
	//-----------------------BODY--------------------//

	glColor3f(r / 255.0, g / 255.0, b / 255.0);	//	Blue
	glBegin(GL_POLYGON);

	double sz = fishSize;
	double theta = atan2(dy, dx);

	Point p = Point(0, -sz / 2);
	p.rotate(theta);
	glVertex2d(coord.x + p.x, coord.y + p.y);

	p = Point(sz, 0);
	p.rotate(theta);
	glVertex2d(coord.x + p.x, coord.y + p.y);

	p = Point(0, sz / 2);
	p.rotate(theta);
	glVertex2d(coord.x + p.x, coord.y + p.y);

	p = Point(-sz, 0);
	p.rotate(theta);
	glVertex2d(coord.x + p.x, coord.y + p.y);

	glEnd();

	//-----------------------TAIL--------------------//

	glColor3f(r / 255.0, g / 255.0, b / 255.0);	//	Blue
	glBegin(GL_POLYGON);

	p = Point(-sz * 1.3, sz / 2);
	p.rotate(theta);
	glVertex2d(coord.x + p.x, coord.y + p.y);

	p = Point(-sz * 1.3, -sz / 2);
	p.rotate(theta);
	glVertex2d(coord.x + p.x, coord.y + p.y);

	p = Point(-sz * 0.9, 0);
	p.rotate(theta);
	glVertex2d(coord.x + p.x, coord.y + p.y);

	glEnd();
}

void Fish::drawShark()
{
	//-----------------------BODY--------------------//
	//	128
	glColor3f(r / 255.0, g / 255.0, b / 255.0); // Gray
	glBegin(GL_POLYGON);

	double theta = atan2(dy, dx);
	double sz = sharkSize;
	double cte = sz * 1;

	Point p = Point(-sz * 1.5 + cte, -sz * 0.35);
	p.rotate(theta);
	glVertex2d(coord.x + p.x, coord.y + p.y);

	p = Point(0 + cte, -sz * 0.5);
	p.rotate(theta);
	glVertex2d(coord.x + p.x, coord.y + p.y);

	p = Point(sz * 0.5 + cte, -sz * 0.25);
	p.rotate(theta);
	glVertex2d(coord.x + p.x, coord.y + p.y);

	p = Point(sz * 0.70 + cte, 0);
	p.rotate(theta);
	glVertex2d(coord.x + p.x, coord.y + p.y);

	p = Point(sz * 0.5 + cte, sz * 0.25);
	p.rotate(theta);
	glVertex2d(coord.x + p.x, coord.y + p.y);

	p = Point(0 + cte, sz * 0.5);
	p.rotate(theta);
	glVertex2d(coord.x + p.x, coord.y + p.y);

	p = Point(-sz * 1.5 + cte, sz * 0.35);
	p.rotate(theta);
	glVertex2d(coord.x + p.x, coord.y + p.y);

	p = Point(-sz * 2.5 + cte, 0);
	p.rotate(theta);
	glVertex2d(coord.x + p.x, coord.y + p.y);

	glEnd();

	//-----------------------TAIL--------------------//

	glColor3f(r / 255.0, g / 255.0, b / 255.0);
	glBegin(GL_POLYGON);

	//double cnt = sz * 2.5;

	p = Point(-sz * 2.6 + cte, sz * 0.2);
	p.rotate(theta);
	glVertex2d(coord.x + p.x, coord.y + p.y);

	p = Point(-sz * 3.0 + cte, 0);
	p.rotate(theta);
	glVertex2d(coord.x + p.x, coord.y + p.y);

	p = Point(-sz * 2.6 + cte, -sz * 0.2);
	p.rotate(theta);
	glVertex2d(coord.x + p.x, coord.y + p.y);

	p = Point(-sz * 2.2 + cte, 0);
	p.rotate(theta);
	glVertex2d(coord.x + p.x, coord.y + p.y);

	glEnd();

	//-----------------------FINS--------------------//

	glColor3f(r / 255.0, g / 255.0, b / 255.0);
	glBegin(GL_POLYGON);

	p = Point(0 + cte, 0);
	p.rotate(theta);
	glVertex2d(coord.x + p.x, coord.y + p.y);

	p = Point(-sz + cte, -sz * 1.2);
	p.rotate(theta);
	glVertex2d(coord.x + p.x, coord.y + p.y);

	p = Point(-sz * 0.6 + cte, 0);
	p.rotate(theta);
	glVertex2d(coord.x + p.x, coord.y + p.y);

	p = Point(-sz + cte, sz * 1.2);
	p.rotate(theta);
	glVertex2d(coord.x + p.x, coord.y + p.y);

	glEnd();
}