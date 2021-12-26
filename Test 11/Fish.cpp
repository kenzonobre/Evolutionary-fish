#include"Fish.h"

Fish::Fish(double _x, double _y, double _dx, double _dy, bool _isShark)
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
		//	change rand() later because it is not random hahah
		dx += (float) (rand() % (2 * sharkSpeed) - sharkSpeed) / 50000;
		dy += (float) (rand() % (2 * sharkSpeed) - sharkSpeed) / 50000;
	}
	else
	{
		dx += (float) (rand() % (2 * fishSpeed) - fishSpeed) / 50000;
		dy += (float) (rand() % (2 * fishSpeed) - fishSpeed) / 50000;
	}

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

void Fish::alignment(vector<Fish> neighbors, int8 alignmentWeigh)
{
	float radius = fishVision * fishSize;
	float averageDx = 0, averageDy = 0;
	int countNeighbors = 0;

	for(auto neighbor : neighbors)
		if(getDistance(neighbor) <= radius)
		{
			averageDx += neighbor.dx;
			averageDy += neighbor.dy;
			countNeighbors++;
		}

	averageDx /= countNeighbors;
	averageDy /= countNeighbors;

	dx += averageDx * alignmentWeigh / 1000;
	dy += averageDy * alignmentWeigh / 1000;
}

void Fish::cohesion(vector<Fish> neighbors, int8 cohesionWeigh)
{
	float radius = fishVision * fishSize;
	float centerX = 0, centerY = 0;
	int countNeighbors = 0;

	for(auto neighbor : neighbors)
		if(getDistance(neighbor) <= radius)
		{
			centerX += neighbor.coord.x;
			centerY += neighbor.coord.y;
			countNeighbors++;
		}

	centerX /= countNeighbors;
	centerY /= countNeighbors;

	dx += (centerX - (coord).x) * cohesionWeigh / 1000;
	dy += (centerY - (coord).y) * cohesionWeigh / 1000;
}

void Fish::separation(vector<Fish> neighbors, int8 separationWeigh)
{	
	float radius = 2 * fishSize;
	float avoidDx = 0, avoidDy = 0;
	int countNeighbors = 0;

	for(auto neighbor : neighbors)
		if(getDistance(neighbor) <= radius)
		{
			avoidDx += coord.x - neighbor.coord.x;
			avoidDy += coord.y - neighbor.coord.y;
			countNeighbors++;
		}

	dx += avoidDx * separationWeigh / 1000;
	dy += avoidDy * separationWeigh / 1000;
}

void Fish::matchSpeed(vector<Fish> neighbors)
{
	float radius = fishVision * fishSize;
	float averageSpeed = 0;
	int countNeighbors = 0;

	for(auto neighbor : neighbors)
		if(getDistance(neighbor) <= radius)
		{
			averageSpeed += neighbor.getSpeed();
			countNeighbors++;
		}

	averageSpeed /= countNeighbors;
	float speed = getSpeed();

	dx *= (averageSpeed / speed);
	dy *= (averageSpeed / speed);
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

void Fish::avoidSharks(vector<Fish> sharks)
{
	float radius = fishVision * fishSize;
	float avoidDx = 0, avoidDy = 0;

	for(auto shark : sharks)
		if(getDistance(shark) <= radius)
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

	for(int i = 0; i < (int) neighbors.size(); i++)
	{
		if(getDistance(neighbors[i]) <= radius)
		{
			float fishDx = neighbors[i].coord.x - coord.x;
			float fishDy = neighbors[i].coord.y - coord.y;

			float angle = atan2(fishDy, fishDx);
			float sharkAngle = atan2(dy, dx);

			if(angle < 0) angle += 2 * M_PI;
			if(sharkAngle < 0)	sharkAngle += 2 * M_PI;

			if(abs(angle - sharkAngle) <= M_PI * 3 / 2)
			{
				centerX += neighbors[i].coord.x;
				centerY += neighbors[i].coord.y;
				countNeighbors++;
			}
		}
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
	if(isShark)	return false;

	for(int i = 0; i < (int) sharks.size(); i++)
	{
		//	4 Points that forms the shark's head
		vector<Point> head(4);
		head[0] = Point(sharkSize, -sharkSize * 0.5);
		head[1] = Point(sharkSize * 1.70, -sharkSize * 0.5);
		head[2] = Point(sharkSize * 1.70, sharkSize * 0.5);
		head[3] = Point(sharkSize, sharkSize * 0.5);

		for(int j = 0; j < 4; j++)
		{
			head[j].rotate(atan2(sharks[i].dy, sharks[i].dx));
			head[j] = head[j] + sharks[i].coord;
		}

		//	Checking if the Fish is inside the shark's head
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

	glColor3f(gray, gray, gray);
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