#include <stdio.h>
#include <stdlib.h>
#include <gl\glut.h>

//////////////////
/*	VARIABLES	*/
//////////////////

#define OBSERWATOR_FOV_Y        30.0  

double maxDepth = 1000;
double minDepth = 1;

int     windowWidth = 1024;
int     windowHeight = 768;


GLfloat depth = 50.0;
GLfloat positionZ = 0.0;
GLfloat positionY = 20.0;
GLfloat positionX = 20.0;

GLfloat knots[8] = { 0.0, 0.0, 0.0, 0.0, 0.8, 0.8, 0.8, 0.8 };
GLfloat pts1[4][4][3], pts2[4][4][3];
GLfloat pts3[4][4][3], pts4[4][4][3];
GLUnurbsObj *nurb;
int u, v;

//////////////////////////
/*	DRAW AXIS FUNCTION	*/
//////////////////////////

void DrawAxis()
{
	glLineWidth(1.0f);
	glBegin(GL_LINES);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-100.0, 0.0, 0.0);
	glVertex3f(100.0, 0.0, 0.0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, -100.0, 0.0);
	glVertex3f(0.0, 100.0, 0.0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, -100.0);
	glVertex3f(0.0, 0.0, 100.0);

	glEnd();
}

//////////////////////////////
/*	WINDOW RESIZE FUNCTION	*/
//////////////////////////////

void WindowResize(int width, int height)
{
	windowWidth = width;
	windowHeight = height;

	glViewport(0, 0, windowWidth, windowHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(OBSERWATOR_FOV_Y, (float)windowWidth / (float)windowHeight, 1.0, 1000.0);
}

//////////////////////////////////
/*	NURBS ALGORITHM FUNCTION	*/
//////////////////////////////////

void DrawNurbs(){

	glColor3f(1.0, 0, 1.0);
	//glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	nurb = gluNewNurbsRenderer();
	gluNurbsProperty(nurb, GLU_SAMPLING_TOLERANCE, 25.0);
	gluNurbsProperty(nurb, GLU_DISPLAY_MODE, GLU_OUTLINE_POLYGON);


	for (u = 0; u<4; u++) {
		for (v = 0; v<4; v++) {

			pts1[u][v][0] = 2.0*((GLfloat)u);
			pts1[u][v][1] = 2.0*((GLfloat)v);
			if ((u == 1 || u == 2) && (v == 1 || v == 2))

				pts1[u][v][2] = 6.0;
			else
				pts1[u][v][2] = 0.0;


			pts2[u][v][0] = 2.0*((GLfloat)u - 3.0);
			pts2[u][v][1] = 2.0*((GLfloat)v - 3.0);
			if ((u == 1 || u == 2) && (v == 1 || v == 2))
			if (u == 1 && v == 1)

				pts2[u][v][2] = 15.0;
			else

				pts2[u][v][2] = -2.0;
			else
				pts2[u][v][2] = 0.0;


			pts3[u][v][0] = 2.0*((GLfloat)u - 3.0);
			pts3[u][v][1] = 2.0*((GLfloat)v);
			if ((u == 1 || u == 2) && (v == 1 || v == 2))
			if (u == 1 && v == 2)

				pts3[u][v][2] = 11.0;
			else

				pts3[u][v][2] = 2.0;
			else
				pts3[u][v][2] = 0.0;


			pts4[u][v][0] = 2.0*((GLfloat)u);
			pts4[u][v][1] = 2.0*((GLfloat)v - 3.0);
			if ((u == 1 || u == 2 || u == 3) && (v == 1 || v == 2))
			if (v == 1)

				pts4[u][v][2] = -2.0;
			else

				pts4[u][v][2] = 5.0;
			else
				pts4[u][v][2] = 0.0;
		}
	}

	pts1[3][3][2] = 6;

	pts2[0][0][2] = -2;

	pts1[0][0][2] = 1;
	pts2[3][3][2] = 1;
	pts3[3][0][2] = 1;
	pts4[0][3][2] = 1;

	glMatrixMode(GL_PROJECTION);
	//gluPerspective(55.0, 1.0, 2.0, 24.0);
	glMatrixMode(GL_MODELVIEW);
	//glTranslatef(0.0, 0.0, -15.0);
	//glRotatef(330.0, 1.0, 0.0, 0.0);

	gluBeginSurface(nurb);
	gluNurbsSurface(nurb, 8, knots, 8, knots,
		4 * 3, 3, &pts1[0][0][0],
		4, 4, GL_MAP2_VERTEX_3);
	gluEndSurface(nurb);

	gluBeginSurface(nurb);
	gluNurbsSurface(nurb, 8, knots, 8, knots,
		4 * 3, 3, &pts2[0][0][0],
		4, 4, GL_MAP2_VERTEX_3);
	gluEndSurface(nurb);


	gluBeginSurface(nurb);
	gluNurbsSurface(nurb, 8, knots, 8, knots,
		4 * 3, 3, &pts3[0][0][0],
		4, 4, GL_MAP2_VERTEX_3);
	gluEndSurface(nurb);


	gluBeginSurface(nurb);
	gluNurbsSurface(nurb, 8, knots, 8, knots,
		4 * 3, 3, &pts4[0][0][0],
		4, 4, GL_MAP2_VERTEX_3);
	gluEndSurface(nurb);


}

//////////////////////////////////////
/*	KEYBOARD SPECIAL KEYS FUNCTION	*/
//////////////////////////////////////

void KeyboardSpecialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		if (positionX < 360) positionX += 1.0;
		else positionX = 0;
		break;

	case GLUT_KEY_DOWN:
		if (positionX > 0) positionX -= 1.0;
		else positionX = 360;
		break;

	case GLUT_KEY_LEFT:
		if (positionY > 0) positionY -= 1.0;
		else positionY = 360;
		break;

	case GLUT_KEY_RIGHT:
		if (positionY < 360) positionY += 1.0;
		else positionY = 0;
		break;
	}


}

//////////////////////////////
/*	KEYBOARD KEYS FUNCTION	*/
//////////////////////////////

void KeyboardKeys(unsigned char key, int x, int y)
{
	switch (key)
	{

	case '/':
		if (positionZ < 360) positionZ += 1.0;
		else positionZ = 0;
		break;

	case '*':
		if (positionZ > 0) positionZ -= 1.0;
		else positionZ = 360;
		break;

	case '-':
		depth = (depth < maxDepth) ? depth + 1.0 : depth;
		break;

	case '+':
		depth = (depth > minDepth) ? depth - 1.0 : depth;
		break;

	}

	if (key == 27)
		exit(0);

}

//////////////////////////
/*	DISPLAY FUNCTION	*/
//////////////////////////

void Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glTranslatef(0, 0, -depth);
	glRotatef(positionX, 1, 0, 0);
	glRotatef(positionY, 0, 1, 0);
	glRotatef(positionZ, 0, 0, 1);

	DrawAxis();

	DrawNurbs();

	glutSwapBuffers();
}

//////////////////////
/*	MAIN FUNCTION	*/
//////////////////////

int main(int argc, char **argv){

glutInit(&argc, argv);

glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

glutInitWindowPosition(100, 100);

glutInitWindowSize(windowWidth, windowHeight);

glutCreateWindow("NURBS");

glEnable(GL_DEPTH_TEST);

glClearDepth(1000.0);

glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

glutDisplayFunc(Display);

glutReshapeFunc(WindowResize);

glutIdleFunc(Display);

glutKeyboardFunc(KeyboardKeys);

glutSpecialFunc(KeyboardSpecialKeys);

glutMainLoop();

return 0;
}