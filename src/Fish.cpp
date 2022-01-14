#include"Fish.h"

//	Fish Constructor
Fish::Fish(float _x, float _y, float _dx, float _dy, bool _isShark)
{
	this->coord = Point(_x, _y);
	this->dx = _dx;
	this->dy = _dy;
	this->isShark = _isShark;
}

Fish::~Fish()
{
	return;
}

void Fish::move()
{	
	//	Random slight move that each fish/shark does to make its movement more natural 
	if(isShark)
	{	
		this->dx += (float) ((getRand() % (2 * SHARK_MAX_SPEED)) - SHARK_MAX_SPEED) / 20000.0;
		this->dy += (float) ((getRand() % (2 * SHARK_MAX_SPEED)) - SHARK_MAX_SPEED) / 20000.0;
	}
	else
	{
		this->dx += (float) ((getRand() % (2 * FISH_MAX_SPEED)) - FISH_MAX_SPEED) / 10000.0;
		this->dy += (float) ((getRand() % (2 * FISH_MAX_SPEED)) - FISH_MAX_SPEED) / 10000.0;
	}

	float margin = 5 * FISH_SIZE;
	double turnFactor = 0.002;

	if(this->isShark)
	{
		margin = 2 * SHARK_SIZE;
		turnFactor = 0.002;
	}

	//	Avoinding the environment boundaries
	if(this->coord.x < -1 + margin)	this->dx += turnFactor;
	if(this->coord.x > 1 - margin)	this->dx -= turnFactor;
	if(this->coord.y < -1 + margin)	this->dy += turnFactor;
	if(this->coord.y > 0.7 - margin)this->dy -= turnFactor;

	//	Moving fish/shark 
	this->coord.x += this->dx;
	this->coord.y += this->dy;

	//	Stopping fish/shark from overstepping the environment boundaries
	this->coord.x = min((float) 1, this->coord.x);
	this->coord.x = max((float) -1, this->coord.x);
	this->coord.y = min((float) 0.7, this->coord.y);
	this->coord.y = max((float) -1, this->coord.y);
}

bool Fish::insideRangeVision(Fish neighbor)
{
	float theta1 = Point(this->dx, this->dy).angle();
	float theta2 = (neighbor.coord - this->coord).angle();
	float difference = min(abs(theta1 - theta2), (float) (2 * M_PI) - abs(theta1 - theta2));

	return (difference <= (ANGLE_OF_VISION / 360.0) * M_PI);
}

float Fish::getSpeed()
{
	Point direction(this->dx, this->dy);
	return sqrt(direction % direction);
}

float Fish::getDistance(Fish neighbor)
{
	Point direction = neighbor.coord - this->coord;
	return sqrt(direction % direction);
}

void Fish::alignment(vector<Fish> neighbors, int8 alignmentWeigh)
{
	float radius = FISH_DISTANCE_VISION * FISH_SIZE;
	float averageDx = 0, averageDy = 0;
	int countNeighbors = 0;

	for(auto neighbor : neighbors)
		if(this->getDistance(neighbor) <= radius && this->insideRangeVision(neighbor))
		{
			averageDx += neighbor.dx;
			averageDy += neighbor.dy;
			countNeighbors++;
		}

	if(countNeighbors)
	{
		averageDx /= countNeighbors;
		averageDy /= countNeighbors;

		this->dx += averageDx * alignmentWeigh / 1000;
		this->dy += averageDy * alignmentWeigh / 1000;
	}
}

void Fish::cohesion(vector<Fish> neighbors, int8 cohesionWeigh)
{
	float radius = FISH_DISTANCE_VISION * FISH_SIZE;
	float centerX = 0, centerY = 0;
	int countNeighbors = 0;

	for(auto neighbor : neighbors)
		if(this->getDistance(neighbor) <= radius && this->insideRangeVision(neighbor))
		{
			centerX += neighbor.coord.x;
			centerY += neighbor.coord.y;
			countNeighbors++;
		}

	if(countNeighbors)
	{
		centerX /= countNeighbors;
		centerY /= countNeighbors;

		this->dx += (centerX - (coord).x) * cohesionWeigh / 1000;
		this->dy += (centerY - (coord).y) * cohesionWeigh / 1000;
	}
}

void Fish::separation(vector<Fish> neighbors, int8 separationWeigh)
{	
	float radius = 2 * FISH_SIZE;
	float avoidDx = 0, avoidDy = 0;
	int countNeighbors = 0;

	for(auto neighbor : neighbors)
		if(this->getDistance(neighbor) <= radius && this->insideRangeVision(neighbor))
		{
			avoidDx += this->coord.x - neighbor.coord.x;
			avoidDy += this->coord.y - neighbor.coord.y;
			countNeighbors++;
		}

	this->dx += avoidDx * separationWeigh / 1000;
	this->dy += avoidDy * separationWeigh / 1000;
}

void Fish::limitSpeed()
{
	float maxSpeed = (isShark) ? (float) SHARK_MAX_SPEED / 1000 : (float) FISH_MAX_SPEED / 1000; 
	float Speed = this->getSpeed();

	if(Speed > maxSpeed)
	{
		this->dx *= (maxSpeed / Speed);
		this->dy *= (maxSpeed / Speed);
	}
}

void Fish::avoidSharks(vector<Fish> sharks, int8 avoidSharkWeigh)
{
	float radius = FISH_DISTANCE_VISION * FISH_SIZE;
	float avoidDx = 0, avoidDy = 0;

	for(auto shark : sharks)
		if(this->getDistance(shark) <= radius && this->insideRangeVision(shark))
		{
			avoidDx = this->coord.x - shark.coord.x;
			avoidDy = this->coord.y - shark.coord.y;
			radius = this->getDistance(shark);
		}

	dx += avoidDx * avoidSharkWeigh / 1000;
	dy += avoidDy * avoidSharkWeigh / 1000;
}

void Fish::chaseFish(vector<Fish> neighbors)
{
	//	This method is only used if the individual is a shark
	if(!isShark)	return;

	float radius = SHARK_DISTANCE_VISION * SHARK_SIZE;
	float centerX = 0, centerY = 0;
	int countNeighbors = 0;

	for(auto neighbor : neighbors)
		if(this->getDistance(neighbor) <= radius && this->insideRangeVision(neighbor))
		{
			centerX = neighbor.coord.x;
			centerY = neighbor.coord.y;
			countNeighbors++;
			radius = this->getDistance(neighbor);
		}

	if(countNeighbors)
	{	
		this->dx += (centerX - (coord).x) * 5 / 1000;
		this->dy += (centerY - (coord).y) * 5 / 1000;
	}
}

bool Fish::eatenByShark(vector<Fish> sharks)
{	
	//	This method is only used if the individual is not a shark
	if(isShark)	return false;

	for(auto shark : sharks)
	{
		//	4 points that forms the shark's head
		vector<Point> head(4);
		head[0] = Point(SHARK_SIZE, -SHARK_SIZE * 0.5);
		head[1] = Point(SHARK_SIZE * 1.70, -SHARK_SIZE * 0.5);
		head[2] = Point(SHARK_SIZE * 1.70, SHARK_SIZE * 0.5);
		head[3] = Point(SHARK_SIZE, SHARK_SIZE * 0.5);

		float sharkAngle = Point(shark.dx, shark.dy).angle();
		for(int j = 0; j < 4; j++)
		{
			head[j].rotate(sharkAngle);
			head[j] = head[j] + shark.coord;
		}

		//	Checking if the fish is inside the shark's head
		if((this->coord).insideRectangle(head))
			return true;
	}

	return false;
}

void Fish::drawFish(int8 r, int8 g, int8 b)
{
	float red = (float) r / 255.0;
	float green = (float) g / 255.0;
	float blue = (float) b / 255.0;

	float size = FISH_SIZE;
	float theta = atan2(this->dy, this->dx);
	vector<Point> p(4);


	//####################### BODY #######################//
	glColor3f(red, green, blue);	
	glBegin(GL_POLYGON);

	p[0] = Point(0, -0.5 * size);
	p[1] = Point(size, 0);
	p[2] = Point(0, 0.5 * size);
	p[3] = Point(-size, 0);

	for(int i = 0; i < 4; i++)
	{
		p[i].rotate(theta);
		p[i] = p[i] + this->coord;
		glVertex2d(p[i].x, p[i].y);
	}

	glEnd();
	//####################################################//


	//####################### TAIL #######################//
	glColor3f(red, green, blue);
	glBegin(GL_POLYGON);

	p[0] = Point(-1.3 * size, 0.5 * size);
	p[1] = Point(-1.3 * size, -0.5 * size);
	p[2] = Point(-0.9 * size, 0);

	for(int i = 0; i < 3; i++)
	{
		p[i].rotate(theta);
		p[i] = p[i] + this->coord;
		glVertex2d(p[i].x, p[i].y);
	}

	glEnd();
	//####################################################//
}

void Fish::drawShark()
{
	float gray = 128.0 / 255.0;

	float theta = atan2(dy, dx);
	float size = SHARK_SIZE;
	vector<Point> p(8);


	//####################### BODY #######################//
	glColor3f(gray, gray, gray);
	glBegin(GL_POLYGON);

	p[0] = Point(-0.5 * size, -0.35 * size);
	p[1] = Point(size, -0.5 * size);
	p[2] = Point(1.5 * size, -0.25 * size);
	p[3] = Point(1.70 * size, 0);
	p[4] = Point(1.5 * size, 0.25 * size);
	p[5] = Point(size, 0.5 * size);
	p[6] = Point(-0.5 * size, 0.35 * size);
	p[7] = Point(-1.5 * size, 0);

	for(int i = 0; i < 8; i++)
	{
		p[i].rotate(theta);
		p[i] = p[i] + this->coord;
		glVertex2d(p[i].x, p[i].y);
	}

	glEnd();
	//###################################################//


	//####################### TAIL #######################//
	glColor3f(gray, gray, gray);
	glBegin(GL_POLYGON);

	p[0] = Point(-1.6 * size, 0.2 * size);
	p[1] = Point(-2.0 * size, 0);
	p[2] = Point(-1.6 * size, -0.2 * size);
	p[3] = Point(-1.2 * size, 0);

	for(int i = 0; i < 4; i++)
	{
		p[i].rotate(theta);
		p[i] = p[i] + this->coord;
		glVertex2d(p[i].x, p[i].y);
	}

	glEnd();
	//###################################################//


	//####################### FINS ######################//
	glColor3f(gray, gray, gray);
	glBegin(GL_POLYGON);

	p[0] = Point(size, 0);
	p[1] = Point(0, -1.2 * size);
	p[2] = Point(0.4 * size, 0);
	p[3] = Point(0, 1.2 * size);

	for(int i = 0; i < 4; i++)
	{
		p[i].rotate(theta);
		p[i] = p[i] + this->coord;
		glVertex2d(p[i].x, p[i].y);
	}

	glEnd();
	//###################################################//
}

Fish generateRandomFish(bool isShark)
{
	float x = (float) (getRand() % 250) / 1000 + 0.75;
	float y = (float) (getRand() % 700) / 1000;
	float dx = (float) (getRand() % FISH_MAX_SPEED) / 1000;
	float dy = (float) (getRand() % FISH_MAX_SPEED) / 1000;

	if(isShark)			x *= -1;
	if(getRand() & 1)	y *= -1;

	if(getRand() & 1)	dx *= -1;
	if(getRand() & 1)	dy *= -1;

	return Fish(x, y, dx, dy, isShark);
}