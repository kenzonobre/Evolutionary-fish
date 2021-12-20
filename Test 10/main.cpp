#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

#include <iostream>
#include <random>
#include <chrono>
#include "Boids.h"

using namespace std;

#define windowWidth 800
#define windowHeight 800
#define initialNumberOfBoids 30
#define boidsSize 20

int timeCounter = 0;
int cohesionWeigh = 15;
int alignmentWeigh = 15;
int separationWeigh = 15;

vector<Boids> boids;
vector<Fish> sharks;

int Rand(int l = 0, int r = 1000000007)
{
	mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
	return uniform_int_distribution<int>(l, r)(rng);
}

void initializeBoids()
{
	boids.clear();
	boids.resize(initialNumberOfBoids);

	for(int i = 0; i < initialNumberOfBoids; i++)
	{
		int8 r = 0, g = 0, b = 0;
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

		vector<Fish> fish;

		for(int j = 0; j < boidsSize; j++)
			fish.push_back(Fish(x, y, dx, dy, false));

		boids[i] = Boids(fish, r, g, b, Rand() % 100, Rand() % 100, Rand() % 100);
	}
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

		sharks.push_back(Fish(mouseX, mouseY, dx, dy, true));
	}
}

int cntDeadFish = 0;

//------------------ TIMER -----------------//
void timer(int)
{
	timeCounter++;

	for(int i = 0; i < (int) boids.size(); i++)
	{
		for(int j = 0; j < (int) boids[i].fish.size(); j++)
			if(boids[i].fish[j].eatenByShark(sharks))
			{
				printf("a Fish died :(   (counter of Dead Fish = %d)\n", ++cntDeadFish);
				swap(boids[i].fish[j], boids[i].fish.back());
				boids[i].fish.pop_back();
			}

		if(boids[i].fish.empty())
		{
			swap(boids[i], boids.back());
			boids.pop_back();
		}
	}

	vector<Fish> allFish;

	for(int i = 0; i < (int) boids.size(); i++)
	{
		for(int j = 0; j < (int) boids[i].fish.size(); j++)
		{
			boids[i].fish[j].cohesion(boids[i].fish, boids[i].cohesionWeigh);
			boids[i].fish[j].alignment(boids[i].fish, boids[i].alignmentWeigh);
			boids[i].fish[j].separation(boids[i].fish, boids[i].separationWeigh);
			boids[i].fish[j].matchSpeed(boids[i].fish);
			boids[i].fish[j].avoidSharks(sharks);
			boids[i].fish[j].limitSpeed();
			boids[i].fish[j].move();

			allFish.push_back(boids[i].fish[j]);
		}
	}

	for(int i = 0; i < (int) sharks.size(); i++)
	{
		sharks[i].chaseFish(allFish);
		sharks[i].separation(sharks, 50);
		sharks[i].limitSpeed();
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

void drawText(string text, float x, float y, float r, float g, float b)
{
	int sz = text.size();
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	for(int i = 0; i <= sz; i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
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

	text = "[q] INCREASE ALIGNMENT";
	drawText(text, (-1 + 2 * borderSize), (1 - 6 * borderSize), color, color, color);

	text = "[a] DECREASE ALIGNMENT";
	drawText(text, (-1 + 2 * borderSize), (1 - 12 * borderSize), color, color, color);

	text = "ALIGNMENT = " + to_string(alignmentWeigh);
	drawText(text, (-1 + 2 * borderSize), (1 - 26 * borderSize), color, color, color);


	//--------------------------COHESION----------------------------//

	text = "[w] INCREASE COHESION";
	drawText(text, -0.45, (1 - 6 * borderSize), color, color, color);

	text = "[s] DECREASE COHESION";
	drawText(text, -0.45, (1 - 12 * borderSize), color, color, color);

	text = "COHESION = " + to_string(cohesionWeigh);
	drawText(text, -0.45, (1 - 26 * borderSize), color, color, color);


	//--------------------------SEPARATION----------------------------//

	text = "[e] INCREASE SEPARATION";
	drawText(text, 0.1, (1 - 6 * borderSize), color, color, color);

	text = "[d] DECREASE SEPARATION";
	drawText(text, 0.1, (1 - 12 * borderSize), color, color, color);

	text = "SEPARATION = " + to_string(separationWeigh);
	drawText(text, 0.1, (1 - 26 * borderSize), color, color, color);


	//--------------------------RESET----------------------------//

	text = "[r] RESET";
	drawText(text, 0.7, (1 - 6 * borderSize), color, color, color);
}

void Keyboard(unsigned char key, int x, int y)
{
	if(key == 'q')	alignmentWeigh++;
	if(key == 'a')	alignmentWeigh--;

	if(key == 'w')	cohesionWeigh++;
	if(key == 's')	cohesionWeigh--;

	if(key == 'e')	separationWeigh++;
	if(key == 'd')	separationWeigh--;

	if(key == 'r')	
	{
		alignmentWeigh = 15;
		cohesionWeigh = 15;
		separationWeigh = 15;

		sharks.clear();
		initializeBoids();
	}

	if(alignmentWeigh == -1)	alignmentWeigh = 0;
	if(cohesionWeigh == -1)		cohesionWeigh = 0;
	if(separationWeigh == -1)	separationWeigh = 0;
}


//------------------ DRAW -----------------//
void draw()
{
	// Importante: Somente será desenhado o que estiver entre glClear e glEnd
	glClear(GL_COLOR_BUFFER_BIT);

	for(int i = 0; i < (int) boids.size(); i++)
		boids[i].drawBoids();

	for(int i = 0; i < (int) sharks.size(); i++)
		sharks[i].drawShark();

	mainMenu();

	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	initializeBoids();

	//----- Criar janela -----//
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Test 10 - Swarm of Fish");
	glClearColor(153.0 / 255.0, 153.0 / 255.0, 255.0 / 255.0, 0);	// Limpa a tela (red, green, blue, alpha)
	glutDisplayFunc(draw);				// Define qual função irá desenhar
	glutTimerFunc(0, timer, 0);			// Define qual será a função de loop
	glutMouseFunc(mouse);				// É chamada quando ocorre cliques na tela
	glutKeyboardFunc(Keyboard);

	glutMainLoop();

	return 0;
}
