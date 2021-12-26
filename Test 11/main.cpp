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
#define initialNumberOfBoids 20
#define initialNumberOfSharks 10
#define boidsSize 30
#define taxOfMutation 2

//	maior tamanho maior campo de visao
//	maior tamanho menor velocidade

int timeCounter = 0;
int bestAlignment = 0;
int bestCohesion = 0;
int bestSeparation = 0;
int generation = 0;
int cntDeadFish = 0;

vector<Boids> boids;
vector<Fish> sharks;
vector<int>	boidsFitness[5];

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
		boids[i].createBoids(boidsSize);
}

void initializeSharks()
{
	sharks.clear();
	for(int i = 0; i < initialNumberOfSharks; i++)
	{
		float x = (float) (rand() % 1000) / 1000;
		float y = (float) (rand() % 1000) / 1000;
		float dx = (float) (rand() % sharkSpeed) / 1000;
		float dy = (float) (rand() % sharkSpeed) / 1000;

		if(rand() & 1)	dx *= -1;
		if(rand() & 1)	dy *= -1;

		x *= -1;
		if(rand() & 1)	y *= -1;

		sharks.push_back(Fish(x, y, dx, dy, true));
	}
}

void resetEverything()
{
	generation = 0;
	cntDeadFish = 0;
	initializeBoids();
	initializeSharks();
	for(int k = 0; k < 5; k++)
	{
		boidsFitness[k].clear();
		boidsFitness[k].resize(initialNumberOfBoids, 0);
	}
}

void inRange(int8 &value)
{
	if(value < 0)	value = 0;
	if(value > 255)	value = 255;
}

Boids mergeBoids(Boids a, Boids b)
{
	Boids c;
	c.createBoids(boidsSize);

	c.r = (a.r + b.r) >> 1;
	c.g = (a.g + b.g) >> 1;
	c.b = (a.b + b.b) >> 1;

	c.alignmentWeigh = (a.alignmentWeigh + b.alignmentWeigh) >> 1;
	c.cohesionWeigh = (a.cohesionWeigh + b.cohesionWeigh) >> 1;
	c.separationWeigh = (a.separationWeigh + b.separationWeigh) >> 1;

	c.r += ((Rand(0, 1)) ? 1 : -1) * taxOfMutation * 10;
	c.g += ((Rand(0, 1)) ? 1 : -1) * taxOfMutation * 10;
	c.b += ((Rand(0, 1)) ? 1 : -1) * taxOfMutation * 10;
	c.alignmentWeigh += ((Rand(0, 1)) ? 1 : -1) * taxOfMutation;
	c.cohesionWeigh += ((Rand(0, 1)) ? 1 : -1) * taxOfMutation;
	c.separationWeigh += ((Rand(0, 1)) ? 1 : -1) * taxOfMutation;

	inRange(c.r);
	inRange(c.g);
	inRange(c.b);
	inRange(c.alignmentWeigh);
	inRange(c.cohesionWeigh);
	inRange(c.separationWeigh);

	return c;
}

void elitism(int idBestBoid)
{
	vector<Boids> newBoids((int) boids.size());

	for(int i = 0; i < (int) boids.size(); i++)
		newBoids[i] = mergeBoids(boids[i], boids[idBestBoid]);

	boids.clear();
	boids = newBoids;
}

int getAverageFitness(int id)
{	
	int averageFitness = 0;
	for(int k = 0; k < 5; k++)
		averageFitness += boidsFitness[k][id];
	return averageFitness;
}

void evolve()
{
	//	Updating fitness score
	for(int i = 0; i < (int) boids.size(); i++)
		boidsFitness[generation % 5][i] += (int) boids[i].fish.size() * 30;

	//	Getting the index of the best boid
	int idBestBoid = 0;
	for(int i = 1; i < (int) boids.size(); i++)
		if(getAverageFitness(i) > getAverageFitness(idBestBoid))
			idBestBoid = i;

	//	Outputting every fitness score of this generation
	FILE *file = fopen("data.txt", "a");

	for(int i = 0; i < (int) boids.size(); i++)
		fprintf(file, "%d ", boidsFitness[generation % 5][i]);
	fprintf(file, "DeadFish=%d id=%d", cntDeadFish, idBestBoid);
	fprintf(file, "\n");

	fclose(file);

	elitism(idBestBoid);

	bestAlignment = boids[idBestBoid].alignmentWeigh;
	bestCohesion = boids[idBestBoid].cohesionWeigh;
	bestSeparation = boids[idBestBoid].separationWeigh;

	generation++;
	cntDeadFish = 0;

	for(int i = 0; i < (int) boids.size(); i++)
		boidsFitness[generation % 5][i] = 0;
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

//------------------ TIMER -----------------//
void timer(int)
{
	timeCounter++;

	if(timeCounter >= 60 * 30)
	{
		initializeSharks();
		evolve();
		timeCounter = 0;
	}

	for(int i = 0; i < (int) boids.size(); i++)
	{
		for(int j = 0; j < (int) boids[i].fish.size(); j++)
			if(boids[i].fish[j].eatenByShark(sharks))
			{
				printf("a Fish died :(   (counter of Dead Fish = %d)\n", ++cntDeadFish);
				swap(boids[i].fish[j], boids[i].fish.back());
				boids[i].fish.pop_back();
				boidsFitness[generation % 5][i] += timeCounter / 60;
			}
	}

	for(int i = 0; i < (int) boids.size(); i++)
		boids[i].moveBoids(sharks);

	vector<Fish> allFish;
	for(int i = 0; i < (int) boids.size(); i++)
		for(int j = 0; j < (int) boids[i].fish.size(); j++)
			allFish.push_back(boids[i].fish[j]);

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

	//----------------------------------------------------------------//

	text = "ALIGNMENT = " + to_string(bestAlignment);
	drawText(text, (-1 + 6 * borderSize), (1 - 6 * borderSize), color, color, color);

	text = "COHESION = " + to_string(bestCohesion);
	drawText(text, (-1 + 6 * borderSize), (1 - 12 * borderSize), color, color, color);

	text = "SEPARATION = " + to_string(bestSeparation);
	drawText(text, (-1 + 6 * borderSize), (1 - 18 * borderSize), color, color, color);

	text = "GENERATION = " + to_string(generation);
	drawText(text, (-1 + 6 * borderSize), (1 - 24 * borderSize), color, color, color);

	text = "TIMER : " + to_string(30 - (timeCounter / 60));
	drawText(text, -0.5, (1 - 6 * borderSize), color, color, color);

	text = "DEAD FISH : " + to_string(cntDeadFish);
	drawText(text, -0.5, (1 - 12 * borderSize), color, color, color);

/*
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
*/

	//--------------------------RESET----------------------------//

	text = "[e] EVOLVE";
	drawText(text, 0.5, (1 - 6 * borderSize), color, color, color);

	//--------------------------EVOLVE----------------------------//

	text = "[r] RESET";
	drawText(text, 0.5, (1 - 12 * borderSize), color, color, color);
}

void Keyboard(unsigned char key, int x, int y)
{
	if(key == 'r')	
	{
		resetEverything();
	}

	if(key == 'e')
	{
		initializeSharks();
		evolve();
	}
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
	resetEverything();

	//----- Criar janela -----//
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Test 10");
	glClearColor(145.0 / 255.0, 156.0 / 255.0, 255.0 / 255.0, 0);	// Limpa a tela (red, green, blue, alpha)
	glutDisplayFunc(draw);				// Define qual função irá desenhar
	glutTimerFunc(0, timer, 0);			// Define qual será a função de loop
	glutMouseFunc(mouse);				// É chamada quando ocorre cliques na tela
	glutKeyboardFunc(Keyboard);

	glutMainLoop();

	return 0;
}
