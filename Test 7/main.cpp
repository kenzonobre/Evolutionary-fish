#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

#include<bits/stdc++.h>
#include "Point.h"
#include "Fish.h"

using namespace std;

#define windowWidth 800
#define windowHeight 800
#define initialNumberOfSwarms 15
#define swarmSize 20

int fishSpeed = 10;
int sharkSpeed = 10;

int timeCounter = 0;
int cohesionWeigh = 15;
int alignmentWeigh = 15;
int separationWeigh = 15;
int fishVision = 7;

unsigned char r = 0, g = 0, b = 0;
int sizeOfNewSwarm = 0, idNewSwarm = initialNumberOfSwarms;

vector<vector<Fish> > swarm;
vector<Fish> sharks;

int Rand(int l = 0, int r = 1e9)
{
	mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
	return uniform_int_distribution<int>(l, r)(rng);
}

void alignment(Fish& fish, int id)
{
	float radius = fishVision * fishSize;
	float averageDx = 0, averageDy = 0;
	int countNeighbors = 0;

	for(auto neighbor : swarm[id])
		if(fish.getDistance(neighbor) <= radius)
		{
			averageDx += neighbor.dx;
			averageDy += neighbor.dy;
			countNeighbors++;
		}

	if(countNeighbors <= 1)	
		return;

	averageDx /= countNeighbors;
	averageDy /= countNeighbors;

	fish.dx += averageDx * alignmentWeigh / 1000;
	fish.dy += averageDy * alignmentWeigh / 1000;
}

void cohesion(Fish& fish, int id)
{
	float radius = fishVision * fishSize;
	float centerX = 0, centerY = 0;
	int countNeighbors = 0;

	for(auto neighbor : swarm[id])
		if(fish.getDistance(neighbor) <= radius)
		{
			centerX += neighbor.coord.x;
			centerY += neighbor.coord.y;
			countNeighbors++;
		}

	if(countNeighbors <= 1)	
		return;

	centerX /= countNeighbors;
	centerY /= countNeighbors;

	fish.dx += (centerX - (fish.coord).x) * cohesionWeigh / 1000;
	fish.dy += (centerY - (fish.coord).y) * cohesionWeigh / 1000;
}

void separation(Fish& fish, int id)
{	
	float radius = 2 * fishSize;
	float avoidDx = 0, avoidDy = 0;
	int countNeighbors = 0;

	for(auto neighbor : swarm[id])
		if(fish.getDistance(neighbor) <= radius)
		{
			avoidDx += fish.coord.x - neighbor.coord.x;
			avoidDy += fish.coord.y - neighbor.coord.y;
			countNeighbors++;
		}

	fish.dx += avoidDx * separationWeigh / 1000;
	fish.dy += avoidDy * separationWeigh / 1000;
}

void matchSpeed(Fish& fish, int id)
{
	float radius = fishVision * fishSize;
	float averageSpeed = 0;
	int countNeighbors = 0;

	for(auto neighbor : swarm[id])
		if(fish.getDistance(neighbor) <= radius)
		{
			averageSpeed += neighbor.getSpeed();
			countNeighbors++;
		}

	if(countNeighbors <= 1)	
		return;

	averageSpeed /= countNeighbors;
	float speed = fish.getSpeed();

	fish.dx *= (averageSpeed / speed);
	fish.dy *= (averageSpeed / speed);
}

void limitSpeed(Fish& fish)
{
	float maxSpeed = (fish.isShark) ? (float) sharkSpeed / 1000 : (float) fishSpeed / 1000; 
	float Speed = fish.getSpeed();

	if(Speed > maxSpeed)
	{
		fish.dx *= (maxSpeed / Speed);
		fish.dy *= (maxSpeed / Speed);
	}
}

void avoidShark(Fish& fish)
{
	float radius = fishVision * fishSize;
	float avoidDx = 0, avoidDy = 0;

	for(auto shark : sharks)
		if(fish.getDistance(shark) <= radius)
		{
			avoidDx += fish.coord.x - shark.coord.x;
			avoidDy += fish.coord.y - shark.coord.y;
		}

	fish.dx += avoidDx * 50 / 1000;
	fish.dy += avoidDy * 50 / 1000;
}

void chaseFish(Fish& shark)
{
	float radius = 5 * sharkSize;
	float centerX = 0, centerY = 0;
	int countNeighbors = 0;

	for(int i = 0; i < (int) swarm.size(); i++)
	{
		for(int j = 0; j < (int) swarm[i].size(); j++)
		{
			if(shark.getDistance(swarm[i][j]) <= radius)
			{
				float dx = swarm[i][j].coord.x - sharks[i].coord.x;
				float dy = swarm[i][j].coord.y - sharks[i].coord.y;

				float angle = atan2(dy, dx);
				float sharkAngle = atan2(shark.dy, shark.dx);

				if(angle < 0) angle += 2 * M_PI;
				if(sharkAngle < 0)	sharkAngle += 2 * M_PI;

				if(abs(angle - sharkAngle) <= M_PI * 3 / 2)
				{
					centerX += swarm[i][j].coord.x;
					centerY += swarm[i][j].coord.y;
					countNeighbors++;
				}
			}
		}
	}

	if(countNeighbors == 0)	
		return;

	centerX /= countNeighbors;
	centerY /= countNeighbors;

	shark.dx += (centerX - (shark.coord).x) * 5 / 1000;
	shark.dy += (centerY - (shark.coord).y) * 5 / 1000;
}

void initializeSwarms()
{
	for(int i = 0; i < (int) swarm.size(); i++)
		swarm[i].clear();
	swarm.clear();

	swarm.resize(initialNumberOfSwarms);

	for(int i = 0; i < initialNumberOfSwarms; i++)
	{
		r = 0, g = 0, b = 0;
		float x = (float) (Rand() % 1000) / 1000;
		float y = (float) (Rand() % 1000) / 1000;
		float dx = (float) (Rand() % fishSpeed) / 1000;
		float dy = (float) (Rand() % fishSpeed) / 1000;

		if(Rand() & 1)	dx *= -1;
		if(Rand() & 1)	dy *= -1;

		while(r + g + b < 150 || 400 < r + g + b)
		{
			r = Rand() % 256;
			g = Rand() % 256;
			b = Rand() % 256;
		}

		if(Rand() & 1)	x *= -1;
		if(Rand() & 1)	y *= -1;

		for(int j = 0; j < swarmSize; j++)
		{
			swarm[i].push_back(Fish(x, y, dx, dy, false, r, g, b));
		}
	}
}

int cntDeadFish = 0;

bool isDead(Fish fish)
{
	for(int i = 0; i < (int) sharks.size(); i++)
	{
		//	4 Points that forms the shark's head
		vector<Point> head;
		head.push_back(Point(sharkSize, -sharkSize * 0.5));
		head.push_back(Point(sharkSize * 1.70, -sharkSize * 0.5));
		head.push_back(Point(sharkSize * 1.70, sharkSize * 0.5));
		head.push_back(Point(sharkSize, sharkSize * 0.5));

		for(int j = 0; j < 4; j++)
		{
			head[j].rotate(atan2(sharks[i].dy, sharks[i].dx));
			head[j] = head[j] + sharks[i].coord;
		}

		//	Checking if the Fish is inside the shark's head
		if((fish.coord).insideRectangle(head))	
		{
			printf("a Fish died :(   (counter of Dead Fish = %d)\n", ++cntDeadFish);
			return true;
		}
	}

	return false;
}

//------------------ MOUSE -----------------//
void mouse(int button, int state, int x, int y)
{
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		float mouseX = 2 * (((float) x / windowWidth) - 0.5);
		float mouseY = -2 * (((float) y / windowHeight) - 0.5);

		float dx = (float) (Rand() % fishSpeed) / 1000;
		float dy = (float) (Rand() % fishSpeed) / 1000;

		sharks.push_back(Fish(mouseX, mouseY, dx, dy, true, 126, 126, 126));
	}
}

//------------------ TIMER -----------------//
void timer(int)
{
	timeCounter++;
	//printf("timeCounter = %d\n", timeCounter);
	// Essa função é chamada em loop, é aqui que realizaremos as animações

	for(int i = 0; i < (int) swarm.size(); i++)
	{
		for(int j = 0; j < (int) swarm[i].size(); j++)
			if(isDead(swarm[i][j]))
			{
				swap(swarm[i][j], swarm[i].back());
				swarm[i].pop_back();
			}

		if(swarm[i].empty())
		{
			swap(swarm[i], swarm.back());
			swarm.pop_back();
		}
	}

	for(int i = 0; i < (int) swarm.size(); i++)
	{
		for(int j = 0; j < (int) swarm[i].size(); j++)
		{
			swarm[i][j].dx += (float) (Rand() % (2 * fishSpeed) - fishSpeed) / 50000;
			swarm[i][j].dy += (float) (Rand() % (2 * fishSpeed) - fishSpeed) / 50000;
			cohesion(swarm[i][j], i);
			alignment(swarm[i][j], i);
			separation(swarm[i][j], i);
			//matchSpeed(swarm[i][j], i);
			limitSpeed(swarm[i][j]);
			avoidShark(swarm[i][j]);
			swarm[i][j].move();
		}
	}

	for(int i = 0; i < (int) sharks.size(); i++)
	{
		sharks[i].dx += (float) (Rand() % (2 * sharkSpeed) - sharkSpeed) / 50000;
		sharks[i].dy += (float) (Rand() % (2 * sharkSpeed) - sharkSpeed) / 50000;
		chaseFish(sharks[i]);
		limitSpeed(sharks[i]);
		sharks[i].move();
	}

	// Executa a função draw para desenhar novamente
	glutPostRedisplay();

	// O primeiro parâmetro define de quanto em quanto tempo em milissegundos timer será chaamdo
	// Eu coloquei 1000/60 para definir que vai atualizar a 60hz
	glutTimerFunc(1000 / 60, timer, 0);// Garante que esta função será chamada de novo
}

void rectangle(float x, float y, float w, float h, float r, float g, float b)
{
	glColor3f(r, g, b);
	glBegin(GL_POLYGON);

	glVertex2d(x, y);
	glVertex2d(x + w, y);
	glVertex2d(x + w, y - h);
	glVertex2d(x , y - h);

	glEnd();
}

void insertText(string text, float x, float y, float r, float g, float b)
{
	int sz = text.size();
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	for(int i = 0; i <= sz; i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
}

string numberToString(int num)
{
	if(num == 0)	return "0";

	string text = "";

	while(num)
	{
		char c = (num % 10 + '0');
		text = c + text;
		num /= 10;
	}

	return text;
}

void mainMenu()
{
	float color = 96.0 / 255.0;
	rectangle(-1, 1, 2, 0.3, color, color, color);

	color = 64.0 / 255.0;
	float borderSize = 0.01;
	rectangle(-1, 1, borderSize, 0.3, color, color, color);
	rectangle(-1, 1, 2, borderSize, color, color, color);
	rectangle(-1, 0.7 + borderSize, 2, borderSize, color, color, color);
	rectangle(1 - borderSize, 1, borderSize, 0.3, color, color, color);


	color = 255.0 / 255.0;
	string text;

	//--------------------------ALIGNMENT------------------------------//

	text = "[a] INCREASE ALIGNMENT";
	insertText(text, (-1 + 2 * borderSize), (1 - 6 * borderSize), color, color, color);

	text = "[s] DECREASE ALIGNMENT";
	insertText(text, (-1 + 2 * borderSize), (1 - 12 * borderSize), color, color, color);

	text = "ALIGNMENT = " + numberToString(alignmentWeigh);
	insertText(text, (-1 + 2 * borderSize), (1 - 26 * borderSize), color, color, color);


	//--------------------------COHESION----------------------------//

	text = "[d] INCREASE COHESION";
	insertText(text, -0.45, (1 - 6 * borderSize), color, color, color);

	text = "[f] DECREASE COHESION";
	insertText(text, -0.45, (1 - 12 * borderSize), color, color, color);

	text = "COHESION = " + numberToString(cohesionWeigh);
	insertText(text, -0.45, (1 - 26 * borderSize), color, color, color);


	//--------------------------SEPARATION----------------------------//

	text = "[g] INCREASE SEPARATION";
	insertText(text, 0.1, (1 - 6 * borderSize), color, color, color);

	text = "[h] DECREASE SEPARATION";
	insertText(text, 0.1, (1 - 12 * borderSize), color, color, color);

	text = "SEPARATION = " + numberToString(separationWeigh);
	insertText(text, 0.1, (1 - 26 * borderSize), color, color, color);


	//--------------------------RESET----------------------------//

	text = "[r] RESET";
	insertText(text, 0.7, (1 - 6 * borderSize), color, color, color);
}

void Keyboard(unsigned char key, int x, int y)
{
	if(key == 'a')	alignmentWeigh++;
	if(key == 's')	alignmentWeigh--;

	if(key == 'd')	cohesionWeigh++;
	if(key == 'f')	cohesionWeigh--;

	if(key == 'g')	separationWeigh++;
	if(key == 'h')	separationWeigh--;

	if(key == 'r')	
	{
		alignmentWeigh = 15;
		cohesionWeigh = 15;
		separationWeigh = 15;

		for(int i = 0; i < (int) swarm.size(); i++)
			swarm[i].clear();
		swarm.clear();
		sharks.clear();
		initializeSwarms();
	}

	if(fishSpeed == 0)			fishSpeed = 1;
	if(alignmentWeigh == -1)	alignmentWeigh = 0;
	if(cohesionWeigh == -1)		cohesionWeigh = 0;
}


//------------------ DRAW -----------------//
void draw()
{
	// Importante: Somente será desenhado o que estiver entre glClear e glEnd
	glClear(GL_COLOR_BUFFER_BIT);

	for(int i = 0; i < (int) swarm.size(); i++)
		for(int j = 0; j < (int) swarm[i].size(); j++)
			swarm[i][j].drawFish();

	for(int i = 0; i < (int) sharks.size(); i++)
		sharks[i].drawShark();

	mainMenu();

	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	initializeSwarms();

	//----- Criar janela -----//
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Test 7 - Swarm of Fish");
	glClearColor(153.0 / 255.0, 153.0 / 255.0, 255.0 / 255.0, 0);	// Limpa a tela (red, green, blue, alpha)
	glutDisplayFunc(draw);				// Define qual função irá desenhar
	glutTimerFunc(0, timer, 0);			// Define qual será a função de loop
	glutMouseFunc(mouse);				// É chamada quando ocorre cliques na tela
	glutKeyboardFunc(Keyboard);

	glutMainLoop();

	return 0;
}
