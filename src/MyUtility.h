#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

#ifndef MYUTILITY_H
#define MYUTILITY_H

#include <iostream>
#include <math.h>
#include <vector>
#include <random>
#include <chrono>
#include <climits>

using namespace std;

typedef unsigned char int8;

int getRand(int l = 0, int r = INT_MAX);
void limitRange(int8 &value, int8 l = 0, int8 r = 255);
void drawRectangle(float x, float y, float w, float h, float r, float g, float b);
void writeText(string text, float x, float y, float r, float g, float b);

#endif