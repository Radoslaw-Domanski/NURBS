#include <stdio.h>
#include <stdlib.h>
#include <gl\glut.h>
#include <string.h>

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

GLfloat knotsU[8] = { 0.0, 0.0, 0.0, 0.0, 0.8, 0.8, 0.8, 0.8 };
GLfloat knotsV[8] = { 0.0, 0.0, 0.0, 0.0, 0.8, 0.8, 0.8, 0.8 };
GLfloat pts1[4][4][3];
GLfloat pts2[4][4][3];
GLUnurbsObj *nurb;
int u, v;

//////////////////////////
/*	DRAW AXIS FUNCTION	*/
//////////////////////////

void DrawAxis(){
	glLineWidth(1.0f);
	glBegin(GL_LINES);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-20.0, 0.0, 0.0);
	glVertex3f(20.0, 0.0, 0.0);
	glVertex3f(20.0, -2.0, 0);
	glVertex3f(20.0, 2.0, 0.0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, -20.0, 0.0);
	glVertex3f(0.0, 20.0, 0.0);
	glVertex3f(-2.0, 20.0, 0);
	glVertex3f(2.0, 20.0, 0.0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, -20.0);
	glVertex3f(0.0, 0.0, 20.0);
	glVertex3f(0.0, -2.0, 20.0);
	glVertex3f(0.0, 2.0, 20.0);
	glEnd();
}

//////////////////////////////
/*	WINDOW RESIZE FUNCTION	*/
//////////////////////////////

void WindowResize(int width, int height){
	windowWidth = width;
	windowHeight = height;

	glViewport(0, 0, windowWidth, windowHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(OBSERWATOR_FOV_Y, (float)windowWidth / (float)windowHeight, 1.0, 1000.0);
}

///////////////////////////////////////
/*	CALCULATE POINTS IF NOT INCLUDED */
///////////////////////////////////////

void CalculatePoints(){
	for (u = 0; u<4; u++) {
		for (v = 0; v<4; v++) {
			pts1[u][v][0] = 2.0*((GLfloat)u);
			pts1[u][v][1] = 2.0*((GLfloat)v);
			if ((u == 1 || u == 2) && (v == 1 || v == 2))
				pts1[u][v][2] = 6.0;
			else
				pts1[u][v][2] = 0.0;
		}
	}
	pts1[3][3][2] = 6;
	pts1[0][0][2] = 1;
}

//////////////////////////////////
/*	NURBS ALGORITHM FUNCTION	*/
//////////////////////////////////

void DrawNurbs(){

	glColor3f(1.0, 1.0, 1.0);
	//glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	nurb = gluNewNurbsRenderer();
	gluNurbsProperty(nurb, GLU_SAMPLING_TOLERANCE, 25.0);
	gluNurbsProperty(nurb, GLU_DISPLAY_MODE, GLU_OUTLINE_POLYGON);	

	glMatrixMode(GL_PROJECTION);
	glMatrixMode(GL_MODELVIEW);

	/*
	pts2[0][0][0] = 2.0f;
	pts2[0][0][1] = 2.0f;
	pts2[0][0][2] = 0.0f;

	pts2[0][1][0] = 2.0f;
	pts2[0][1][1] = 2.0f;
	pts2[0][1][2] = 4.0f;

	pts2[1][0][0] = 3.0f;
	pts2[1][0][1] = 1.0f;
	pts2[1][0][2] = 0.0f;

	pts2[1][1][0] = 3.0f;
	pts2[1][1][1] = 1.0f;
	pts2[1][1][2] = 4.0f;
	*/







	gluBeginSurface(nurb);
	gluNurbsSurface(nurb, 8, knotsU, 8, knotsV,
		4 * 3, 3, &pts2[0][0][0],
		4, 4, GL_MAP2_VERTEX_3);
	gluEndSurface(nurb);

}

//////////////////////////////////////
/*	KEYBOARD SPECIAL KEYS FUNCTION	*/
//////////////////////////////////////

void KeyboardSpecialKeys(int key, int x, int y){
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

void KeyboardKeys(unsigned char key, int x, int y){
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

void Display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glTranslatef(0, 0, -depth);
	glRotatef(positionX, 1, 0, 0);
	glRotatef(positionY, 0, 1, 0);
	glRotatef(positionZ, 0, 0, 1);

	DrawAxis();
	
	DrawNurbs();
	/*
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 2.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(0.0f, 2.0f, 0.0f);
	glVertex3f(0.0f, 4.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(0.0f, 4.0f, 0.0f);
	glVertex3f(0.0f, 6.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(0.0f, 6.0f, 0.0f);
	glVertex3f(2.0f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(2.0f, 0.0f, 0.0f);
	glVertex3f(2.0f, 2.0f, 6.0f);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(2.0f, 2.0f, 6.0f);
	glVertex3f(2.0f, 4.0f, 6.0f);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(2.0f, 4.0f, 6.0f);
	glVertex3f(2.0f, 6.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(2.0f, 6.0f, 0.0f);
	glVertex3f(4.0f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(4.0f, 0.0f, 0.0f);
	glVertex3f(4.0f, 2.0f, 6.0f);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(4.0f, 2.0f, 6.0f);
	glVertex3f(4.0f, 4.0f, 6.0f);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(4.0f, 4.0f, 6.0f);
	glVertex3f(4.0f, 6.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(4.0f, 4.0f, 6.0f);
	glVertex3f(4.0f, 6.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(4.0f, 6.0f, 6.0f);
	glVertex3f(6.0f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(6.0f, 0.0f, 0.0f);
	glVertex3f(6.0f, 2.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(6.0f, 2.0f, 0.0f);
	glVertex3f(6.0f, 4.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(6.0f, 4.0f, 0.0f);
	glVertex3f(6.0f, 6.0f, 6.0f);
	glEnd();
	*/
	/*
	glBegin(GL_POINTS);
	glColor3f(1.0f, 1.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 2.0f, 0.0f);
	glVertex3f(0.0f, 4.0f, 0.0f);

	glVertex3f(0.0f, 6.0f, 0.0f);
	glVertex3f(2.0f, 0.0f, 0.0f);
	glVertex3f(2.0f, 2.0f, 6.0f);

	glVertex3f(2.0f, 4.0f, 6.0f);
	glVertex3f(2.0f, 6.0f, 0.0f);
	glVertex3f(4.0f, 0.0f, 0.0f);

	glVertex3f(4.0f, 2.0f, 6.0f);
	glVertex3f(4.0f, 4.0f, 6.0f);
	glVertex3f(4.0f, 6.0f, 0.0f);

	glVertex3f(6.0f, 0.0f, 0.0f);
	glVertex3f(6.0f, 2.0f, 6.0f);
	glVertex3f(6.0f, 4.0f, 0.0f);

	glVertex3f(6.0f, 6.0f, 6.0f);

	glEnd();
	*/
	glutSwapBuffers();
}

//////////////////////////////
/*	LOAD POINTS FROM FILE	*/
//////////////////////////////

void insertPoints(char **fileName){
	FILE *pointsFile;
	fopen_s(&pointsFile,fileName, "r");
	const size_t line_size = 300;
	char* line = malloc(line_size);
	char separator[] = " ,;";
	char *tmp;
	int u = 0, v = 0, i = 0;
	char *nextToken = NULL;
	while (fgets(line, line_size, pointsFile) != NULL)  {
			tmp = strtok_s(line, separator, &nextToken);
			while (tmp != NULL){
				pts1[u][v][i] = atof(tmp);
				//printf("pts[%d][%d][%d] : %.2f", u, v, i, pts1[u][v][i]);
				tmp = strtok_s(NULL, separator, &nextToken);
				i++;
				if (i == 3){
					i = 0;
					v++;
					if (v == 4){
						u++;
						v = 0;
						if (u == 4)
							u = 0;
					}
				}
			}
	}
	free(line);
	fclose(pointsFile);
}

//////////////////////////////////////////
/*	INSERT WEIGHTS OF POINTS FROM FILE	*/
//////////////////////////////////////////

void insertWeights(char **fileName){
	FILE *weightsFile;
	fopen_s(&weightsFile, fileName, "r");
	const size_t line_size = 300;
	char* line = malloc(line_size);
	int u = 0, v = 0, i = 0, y = 0;
	while (fgets(line, line_size, weightsFile) != NULL)  {
		//printf("%.2f \n", atof(line));
		for (i = 0; i < 3;i++)
			pts1[u][v][i] = pts1[u][v][i] * atof(line);
		v++;
		if (v == 4){
			u++;
			v = 0;
			if (u == 4)
				u = 0;
		}
	}
	free(line);
	fclose(weightsFile);
}

//////////////////////////////////
/*	INSERT KNOTS U FROM FILE	*/
//////////////////////////////////

void insertKnotsU(char **fileName){
	FILE *KnotsUFile;
	fopen_s(&KnotsUFile, fileName, "r");
	const size_t line_size = 300;
	char* line = malloc(line_size);
	int u = 0, v = 0, i = 0, y = 0;
	char *nextToken = NULL;
	while (fgets(line, line_size, KnotsUFile) != NULL)  {
		//printf("%.2f \n", atof(line));
		knotsU[i] = atof(line);
		i++;
	}
	free(line);
	fclose(KnotsUFile);
}

//////////////////////////////////
/*	INSERT KNOTS V FROM FILE	*/
//////////////////////////////////

void insertKnotsV(char **fileName){
	FILE *KnotsVFile;
	fopen_s(&KnotsVFile, fileName, "r");
	const size_t line_size = 300;
	char* line = malloc(line_size);
	int u = 0, v = 0, i = 0, y = 0;
	char *nextToken = NULL;
	while (fgets(line, line_size, KnotsVFile) != NULL)  {
		//printf("%.2f \n", atof(line));
		knotsV[i] = atof(line);
		i++;
	}
	free(line);
	fclose(KnotsVFile);
}

//////////////////////
/*	MAIN FUNCTION	*/
//////////////////////

int main(int argc, char *argv[]){

	printf("Number of arguments: %d \n", argc);
	int i = 1;
	for (i; i < argc; i++){
		printf("arg%d=%s \n",i,argv[i]);		
	}
	if (argc > 1)
		insertPoints(argv[1]);
	else
		CalculatePoints();
	if (argc > 2)
		insertWeights(argv[2]);
	if (argc > 3)
		insertKnotsU(argv[3]);
	if (argc > 4)
		insertKnotsV(argv[4]);

	for (u = 0; u < 4; u++){
		for (v = 0; v < 4; v++){
			pts2[u][v][0] = pts1[u][v][0];
			pts2[u][v][1] = pts1[u][v][1];
			pts2[u][v][2] = pts1[u][v][2];
		}
	}


	char *myargv[1];
	int myargc = 1;
	myargv[0] =_strdup("NURBS");

	glutInit(&myargc, myargv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowPosition(0, 0);

	glutInitWindowSize(windowWidth, windowHeight);

	glutCreateWindow("NURBS");
	// full screen mode
	//glutFullScreen();

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