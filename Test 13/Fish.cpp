#include"Fish.h"

Fish::Fish(float _x, float _y, float _dx, float _dy, bool _isShark)
{
	coord = Point(_x, _y);
	dx = _dx;
	dy = _dy;
	isShark = _isShark;
}

Fish::~Fish()
{
	return;
}

void Fish::move()
{
	if(isShark)
	{	
		dx += (float) ((getRand() % (2 * sharkSpeed)) - sharkSpeed) / 20000.0;
		dy += (float) ((getRand() % (2 * sharkSpeed)) - sharkSpeed) / 20000.0;
	}
	else
	{
		dx += (float) ((getRand() % (2 * fishSpeed)) - fishSpeed) / 10000.0;
		dy += (float) ((getRand() % (2 * fishSpeed)) - fishSpeed) / 10000.0;
	}

	float margin = 4 * fishSize;
	double turnFactor = 0.002;

	if(isShark)
	{
		margin = 3 * sharkSize;
		turnFactor = 0.001;
	}

	if(coord.x < -1 + margin)
		dx += turnFactor; //max(turnFactor, 0.1 * abs(dx));

	if(coord.x > 1 - margin)
		dx -= turnFactor; //max(turnFactor, 0.1 * abs(dx));

	if(coord.y < -1 + margin)
		dy += turnFactor; //max(turnFactor, 0.1 * abs(dy));

	if(coord.y > 0.7 - margin)
		dy -= turnFactor; //max(turnFactor, 0.1 * abs(dy));

	coord.x += dx;
	coord.y += dy;
}

bool Fish::insideRangeVision(Fish neighbor)
{
	float theta1 = Point(dx, dy).angle();
	float theta2 = (neighbor.coord - coord).angle();

	return (abs(theta1 - theta2) <= (angleRangeVision / 360.0) * M_PI);
}

float Fish::getSpeed()
{
	Point p(dx, dy);
	return sqrt(p % p);
}

float Fish::getDistance(Fish neighbor)
{
	Point p = neighbor.coord - coord;
	return sqrt(p % p);
}

void Fish::alignment(vector<Fish> neighbors, int8 alignmentWeigh)
{
	float radius = fishVision * fishSize;
	float averageDx = 0, averageDy = 0;
	int countNeighbors = 0;

	for(auto neighbor : neighbors)
		if(getDistance(neighbor) <= radius && (*this).insideRangeVision(neighbor))
		{
			averageDx += neighbor.dx;
			averageDy += neighbor.dy;
			countNeighbors++;
		}

	if(countNeighbors)
	{
		averageDx /= countNeighbors;
		averageDy /= countNeighbors;

		dx += averageDx * alignmentWeigh / 1000;
		dy += averageDy * alignmentWeigh / 1000;
	}
}

void Fish::cohesion(vector<Fish> neighbors, int8 cohesionWeigh)
{
	float radius = fishVision * fishSize;
	float centerX = 0, centerY = 0;
	int countNeighbors = 0;

	for(auto neighbor : neighbors)
		if(getDistance(neighbor) <= radius && (*this).insideRangeVision(neighbor))
		{
			centerX += neighbor.coord.x;
			centerY += neighbor.coord.y;
			countNeighbors++;
		}

	if(countNeighbors)
	{
		centerX /= countNeighbors;
		centerY /= countNeighbors;

		dx += (centerX - (coord).x) * cohesionWeigh / 1000;
		dy += (centerY - (coord).y) * cohesionWeigh / 1000;
	}
}

void Fish::separation(vector<Fish> neighbors, int8 separationWeigh)
{	
	float radius = 2 * fishSize;
	float avoidDx = 0, avoidDy = 0;
	int countNeighbors = 0;

	for(auto neighbor : neighbors)
		if(getDistance(neighbor) <= radius && (*this).insideRangeVision(neighbor))
		{
			avoidDx += coord.x - neighbor.coord.x;
			avoidDy += coord.y - neighbor.coord.y;
			countNeighbors++;
		}

	dx += avoidDx * separationWeigh / 1000;
	dy += avoidDy * separationWeigh / 1000;
}

void Fish::limitSpeed()
{
	float maxSpeed = (isShark) ? (float) sharkSpeed / 1000 : (float) fishSpeed / 1000; 
	float Speed = getSpeed();

	if(Speed > maxSpeed)
	{
		dx *= (maxSpeed / Speed);
		dy *= (maxSpeed / Speed);
	}
}

void Fish::avoidSharks(vector<Fish> sharks, int8 avoidSharkWeigh)
{
	float radius = fishVision * fishSize;
	float avoidDx = 0, avoidDy = 0;

	for(auto shark : sharks)
		if(getDistance(shark) <= radius && (*this).insideRangeVision(shark))
		{
			avoidDx += coord.x - shark.coord.x;
			avoidDy += coord.y - shark.coord.y;
		}

	dx += avoidDx * avoidSharkWeigh / 1000;
	dy += avoidDy * avoidSharkWeigh / 1000;
}

void Fish::chaseFish(vector<Fish> neighbors)
{
	float radius = 5 * sharkSize;
	float centerX = 0, centerY = 0;
	int countNeighbors = 0;

	for(auto neighbor : neighbors)
		if(getDistance(neighbor) <= radius && (*this).insideRangeVision(neighbor))
		{
			centerX += neighbor.coord.x;
			centerY += neighbor.coord.y;
			countNeighbors++;
		}

	if(countNeighbors)
	{	
		centerX /= countNeighbors;
		centerY /= countNeighbors;

		dx += (centerX - (coord).x) * 5 / 1000;
		dy += (centerY - (coord).y) * 5 / 1000;
	}
}

bool Fish::eatenByShark(vector<Fish> sharks)
{	
	//	this method is only used if the fish is not a shark
	if(isShark)	return false;

	for(auto shark : sharks)
	{
		//	4 Points that forms the shark's head
		vector<Point> head(4);
		head[0] = Point(sharkSize, -sharkSize * 0.5);
		head[1] = Point(sharkSize * 1.70, -sharkSize * 0.5);
		head[2] = Point(sharkSize * 1.70, sharkSize * 0.5);
		head[3] = Point(sharkSize, sharkSize * 0.5);

		float sharkAngle = Point(shark.dx, shark.dy).angle();
		for(int j = 0; j < 4; j++)
		{
			head[j].rotate(sharkAngle);
			head[j] = head[j] + shark.coord;
		}

		//	Checking if the fish is inside the shark's head
		if((coord).insideRectangle(head))
			return true;
	}

	return false;
}

void Fish::drawFish(int8 r, int8 g, int8 b)
{
	//-----------------------BODY--------------------//

	float red = (float) r / 255;
	float green = (float) g / 255;
	float blue = (float) b / 255;

	glColor3f(red, green, blue);	
	glBegin(GL_POLYGON);

	float sz = fishSize;
	float theta = atan2(dy, dx);

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

	glColor3f(red, green, blue);
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
	float gray = (float) 128 / 255;

	//-----------------------BODY--------------------//

	glColor3f(gray, gray, gray); // Gray
	glBegin(GL_POLYGON);

	float theta = atan2(dy, dx);
	float sz = sharkSize;
	float cte = sz * 1;

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

	glColor3f(gray, gray, gray);
	glBegin(GL_POLYGON);

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

	glColor3f(gray, gray, gray);
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

Fish generateRandomFish(bool isShark)
{
	float x = (float) (getRand() % 500) / 1000 + 0.25;
	float y = (float) (getRand() % 700) / 1000;
	float dx = (float) (getRand() % fishSpeed) / 1000;
	float dy = (float) (getRand() % fishSpeed) / 1000;

	if(isShark)			x *= -1;
	if(getRand() & 1)	y *= -1;

	if(getRand() & 1)	dx *= -1;
	if(getRand() & 1)	dy *= -1;

	return Fish(x, y, dx, dy, isShark);
}