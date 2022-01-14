#include "MyUtility.h"

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

int getRand(int l, int r)
{
	return uniform_int_distribution<int>(l, r)(rng);
}

void limitRange(int8 &value, int8 l, int8 r)
{
	value = max(value, l);
	value = min(value, r);
}

void drawRectangle(float x, float y, float w, float h, float r, float g, float b)
{
	glColor3f(r, g, b);
	glBegin(GL_POLYGON);

	glVertex2d(x, y);
	glVertex2d(x + w, y);
	glVertex2d(x + w, y - h);
	glVertex2d(x , y - h);

	glEnd();
}

void writeText(string text, float x, float y, float r, float g, float b)
{
	int sz = text.size();
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	for(int i = 0; i <= sz; i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
}
