#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

#include <iostream>
#include "Boid.h"

using namespace std;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define NUMBER_OF_BOIDS 20
#define NUMBER_OF_SHARKS 5 

int8 SIMULATION_SPEED = 1;		//	speed of each simulation
int SIMULATION_DURATION = 30;	//	duration of each generation in second

int TAX_OF_MUTATION = 1;		

int TIME_COUNTER = 0;
int GENERATION = 1;
int DEAD_FISH = 0;

int BEST_ALIGNMENT = 0;
int BEST_COHESION = 0;
int BEST_SEPARATION = 0;
int BEST_AVOID_SHARK = 0;

int lastBestBoid = 0;

vector<Boid> boids;
vector<Fish> sharks;
vector<int>	boidsFitness[10];

void initializeBoids()
{
	boids.clear();
	boids.resize(NUMBER_OF_BOIDS);

	for(int i = 0; i < NUMBER_OF_BOIDS; i++)
		boids[i] = generateRandomBoid();
}

void initializeSharks()
{
	sharks.clear();
	sharks.resize(NUMBER_OF_SHARKS);

	for(int i = 0; i < NUMBER_OF_SHARKS; i++)
		sharks[i] = generateRandomFish(true);
}

void resetEverything()
{
	TIME_COUNTER = 0;
	GENERATION = 1;
	DEAD_FISH = 0;
	BEST_ALIGNMENT = 0;
	BEST_COHESION = 0;
	BEST_SEPARATION = 0;
	BEST_AVOID_SHARK = 0;

	initializeBoids();
	initializeSharks();
	
	for(int k = 0; k < 10; k++)
	{
		boidsFitness[k].clear();
		boidsFitness[k].resize(NUMBER_OF_BOIDS, 0);
	}
}

void elitism(int idBestBoid)
{
	vector<Boid> newBoids(NUMBER_OF_BOIDS);

	for(int i = 0; i < NUMBER_OF_BOIDS; i++)
	{
		if(i == idBestBoid || boidsFitness[GENERATION % 10][i] >= boidsFitness[GENERATION % 10][idBestBoid])
		{	
			//	Cloning boid for the next generation
			newBoids[i] = boids[i];
			newBoids[i].fish.resize(BOIDS_SIZE);
			newBoids[i].fish[0] = generateRandomFish(false);
			for(int j = 1; j < BOIDS_SIZE; j++)
				newBoids[i].fish[j] = newBoids[i].fish[0];
		}
		else
		{	
			//	Crossover
			newBoids[i] = boids[i] + boids[idBestBoid];
			
			//	Mutation
			newBoids[i].mutateBoid(TAX_OF_MUTATION);
		}
	}

	boids.clear();
	boids = newBoids;
}

float getAverageFitness(int id)
{	
	float averageFitness = 0;
	int numberOfGENERATIONs = 0;
	int minFitness = 1e9;
	int maxFitness = 0;

	for(int k = 0; k < 10; k++)
		if(boidsFitness[k][id])
		{
			averageFitness += boidsFitness[k][id];
			minFitness = min(minFitness, boidsFitness[k][id]);
			maxFitness = max(maxFitness, boidsFitness[k][id]);
			numberOfGENERATIONs++;
		}

	if(numberOfGENERATIONs >= 3)
		averageFitness = (averageFitness - minFitness - maxFitness) / (numberOfGENERATIONs - 2);
	else
		averageFitness /= numberOfGENERATIONs;

	return averageFitness;
}

void randomPredation(int idWorstBoid)
{
	boids[idWorstBoid] = generateRandomBoid();

	for(int k = 0; k < 10; k++)
		boidsFitness[k][idWorstBoid] = 0;
}

void evolve()
{
	//	Updating fitness score
	for(int i = 0; i < NUMBER_OF_BOIDS; i++)
		boidsFitness[GENERATION % 10][i] += (int) boids[i].fish.size() * SIMULATION_DURATION;

	//	Getting the index of the best boid and the worst boid
	int idBestBoid = 0;
	int idWorstBoid = 0;
	for(int i = 1; i < NUMBER_OF_BOIDS; i++)
	{
		if(getAverageFitness(i) > getAverageFitness(idBestBoid))
			idBestBoid = i;

		if(getAverageFitness(i) < getAverageFitness(idBestBoid))
			idWorstBoid = i;
	}

	if(idBestBoid == lastBestBoid && TAX_OF_MUTATION < 5)
		TAX_OF_MUTATION++;
	else
		TAX_OF_MUTATION = 1;

	lastBestBoid = idBestBoid;

	elitism(idBestBoid);
	if(GENERATION % 10 == 0)
		randomPredation(idWorstBoid);

	BEST_ALIGNMENT = boids[idBestBoid].alignmentWeigh;
	BEST_COHESION = boids[idBestBoid].cohesionWeigh;
	BEST_SEPARATION = boids[idBestBoid].separationWeigh;
	BEST_AVOID_SHARK = boids[idBestBoid].avoidSharkWeigh;

	GENERATION++;
	DEAD_FISH = 0;

	for(int i = 0; i < NUMBER_OF_BOIDS; i++)
		boidsFitness[GENERATION % 10][i] = 0;
}

void timer(int)
{
	TIME_COUNTER++;

	if(TIME_COUNTER >= 60 * SIMULATION_DURATION)
	{
		initializeSharks();
		evolve();
		TIME_COUNTER = 0;
	}

	for(int i = 0; i < NUMBER_OF_BOIDS; i++)
		for(int j = 0; j < (int) boids[i].fish.size(); j++)
			if(boids[i].fish[j].eatenByShark(sharks))
			{
				DEAD_FISH++;
				boidsFitness[GENERATION % 10][i] += (TIME_COUNTER / 60);
				swap(boids[i].fish[j], boids[i].fish.back());
				boids[i].fish.pop_back();
			}

	for(int i = 0; i < NUMBER_OF_BOIDS; i++)
		boids[i].moveBoid(sharks);

	vector<Fish> allFish;
	for(int i = 0; i < NUMBER_OF_BOIDS; i++)
		for(int j = 0; j < (int) boids[i].fish.size(); j++)
			allFish.push_back(boids[i].fish[j]);

	for(int i = 0; i < NUMBER_OF_SHARKS; i++)
	{
		sharks[i].chaseFish(allFish);
		sharks[i].separation(sharks, 100);
		sharks[i].limitSpeed();
		sharks[i].move();
	}

	glutPostRedisplay();
	glutTimerFunc(1000 / (60 * SIMULATION_SPEED), timer, 0);
}

void mainMenu()
{	
	//######################## MENU BACKGROUND ##########################//
	float color = 96.0 / 255.0;
	drawRectangle(-1, 1, 2, 0.3, color, color, color);

	color = 64.0 / 255.0;
	float borderSize = 0.01;
	drawRectangle(-1, 1, borderSize, 0.3, color, color, color);
	drawRectangle(-1, 1, 2, borderSize, color, color, color);
	drawRectangle(-1, 0.7 + borderSize, 2, borderSize, color, color, color);
	drawRectangle(1 - borderSize, 1, borderSize, 0.3, color, color, color);
	//###################################################################//


	//############################# BUTTONS #############################//
	color = 1;
	string text;

	text = "BEST ALIGNMENT : " + to_string(BEST_ALIGNMENT);
	writeText(text, (-1 + 6 * borderSize), (1 - 6 * borderSize), color, color, color);

	text = "BEST COHESION : " + to_string(BEST_COHESION);
	writeText(text, (-1 + 6 * borderSize), (1 - 12 * borderSize), color, color, color);

	text = "BEST SEPARATION : " + to_string(BEST_SEPARATION);
	writeText(text, (-1 + 6 * borderSize), (1 - 18 * borderSize), color, color, color);

	text = "BEST AVOID SHARK : " + to_string(BEST_AVOID_SHARK);
	writeText(text, (-1 + 6 * borderSize), (1 - 24 * borderSize), color, color, color);

	text = "TIMER : " + to_string(SIMULATION_DURATION - (TIME_COUNTER / 60));
	writeText(text, -0.4, (1 - 6 * borderSize), color, color, color);

	text = "DEAD FISH : " + to_string(DEAD_FISH);
	writeText(text, -0.4, (1 - 12 * borderSize), color, color, color);

	text = "GENERATION : " + to_string(GENERATION);
	writeText(text, -0.4, (1 - 24 * borderSize), color, color, color);

	text = "[i] INCREASE SPEED";
	writeText(text, 0.1, (1 - 6 * borderSize), color, color, color);

	text = "[d] DECREASE SPEED";
	writeText(text, 0.1, (1 - 12 * borderSize), color, color, color);

	text = "SPEED : " + to_string(SIMULATION_SPEED);
	writeText(text, 0.1, (1 - 24 * borderSize), color, color, color);

	text = "[e] EVOLVE";
	writeText(text, 0.6, (1 - 6 * borderSize), color, color, color);

	text = "[r] RESET";
	writeText(text, 0.6, (1 - 12 * borderSize), color, color, color);
	//###################################################################//
}

void Keyboard(unsigned char key, int x, int y)
{
	if(key == 'r')	
		resetEverything();

	if(key == 'e')
	{
		initializeSharks();
		evolve();
	}

	if(key == 'i')	SIMULATION_SPEED++;
	if(key == 'd')	SIMULATION_SPEED--;

	limitRange(SIMULATION_SPEED, 1, 4);
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	for(int i = 0; i < NUMBER_OF_BOIDS; i++)
		boids[i].drawBoid();

	for(int i = 0; i < NUMBER_OF_SHARKS; i++)
		sharks[i].drawShark();

	mainMenu();
	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	resetEverything();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Evolutionary fish");
	glClearColor(145.0 / 255.0, 156.0 / 255.0, 255.0 / 255.0, 0);
	glutDisplayFunc(draw);				
	glutTimerFunc(0, timer, 0);
	glutKeyboardFunc(Keyboard);

	glutMainLoop();

	return 0;
}
