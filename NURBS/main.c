#include <stdio.h>
#include <stdlib.h>
#include <gl\glut.h>
#include <string.h>

//////////////////
/*	VARIABLES	*/
//////////////////

struct Surface
{
	GLfloat pts[4][4][3];
	GLfloat knotsU[8];
	GLfloat knotsV[8];
	GLfloat weights[16];
};

#define OBSERWATOR_FOV_Y    30.0  
#define X_OFFSET_SWIATLO    10
#define Y_OFFSET_SWIATLO    120

double maxDepth = 1000;
double minDepth = 1;
struct Surface surfaces[5];

int windowWidth = 1024;
int windowHeight = 768;
int	numberOfSurfaces = 0;

GLfloat depth = 50.0;
GLfloat positionZ = 0.0;
GLfloat positionY = 20.0;
GLfloat positionX = 20.0;

GLUnurbsObj *nurb;
int u, v;
int i, j, k;
////////////////////////////////////////////////////////////////////////

//////////////////////////
/*	DRAW AXIS FUNCTION	*/
//////////////////////////

void DrawAxis()
{
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

void WindowResize(int width, int height)
{
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

void CalculatePoints()
{
	for (u = 0; u<4; u++) 
	{
		for (v = 0; v<4; v++) 
		{
			surfaces[0].pts[u][v][0] = 2.0*((GLfloat)u);
			surfaces[0].pts[u][v][1] = 2.0*((GLfloat)v);
			if ((u == 1 || u == 2) && (v == 1 || v == 2))
				surfaces[0].pts[u][v][2] = 6.0;
			else
				surfaces[0].pts[u][v][2] = 0.0;
		}
	}
	surfaces[0].pts[3][3][2] = 6;
	surfaces[0].pts[0][0][2] = 1;
}

//////////////////////////////////
/*	NURBS ALGORITHM FUNCTION	*/
//////////////////////////////////

void DrawNurbs()
{

	glColor3f(1.0, 1.0, 1.0);
	//glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	nurb = gluNewNurbsRenderer();
	gluNurbsProperty(nurb, GLU_SAMPLING_TOLERANCE, 10.0);
	gluNurbsProperty(nurb, GLU_DISPLAY_MODE, GLU_FILL);	
	//gluNurbsProperty(nurb, GLU_DISPLAY_MODE, GLU_FILL);	
	//gluNurbsProperty(nurb, GLU_DISPLAY_MODE, GLU_OUTLINE_PATCH);	


	glMatrixMode(GL_PROJECTION);
	glMatrixMode(GL_MODELVIEW);

	for (i = 0; i < numberOfSurfaces; i++)
	{
		gluBeginSurface(nurb);
		gluNurbsSurface(nurb, 8, surfaces[i].knotsU, 8, surfaces[i].knotsV,
			4 * 3, 3, &surfaces[i].pts[0][0][0],
			4, 4, GL_MAP2_VERTEX_3);
		gluEndSurface(nurb);
	}
	
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

/*void UstawKolorPozycji(int m, int indeks)
{
	if (m == menu)
	if ((m == ID_MENU_SWIATLA) && (indeks == sIndeks)
		|| (m == ID_MENU_MATERIALU) && (indeks == mIndeks))

		// Pozycja podswietlona wyswietlana jest w kolkorze zoltym
		glColor3f(1.0, 1.0, 0.0);
	else

		// Pozostale na bialo
		glColor3f(1.0, 1.0, 1.0);
}
*/

void RysujTekstRastrowy(void *font, char *tekst)
{
	for (i = 0; i < (int)strlen(tekst); i++)
		glutBitmapCharacter(font, tekst[i]);
}

void RysujNakladke(void)
{
	char buf[255];

	// Zmiana typu rzutu z perspektywicznego na ortogonalny
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, windowWidth, 0.0, windowHeight, -100.0, 100.0);

	// Modelowanie sceny 2D (zawartosci nakladki)
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// Okreslenie koloru tekstu
	glColor3f(1.0, 1.0, 1.0);

	// RYSOWANIE MENU PARAMETROW ZRODLA SWIATLA
	sprintf_s(buf,255, "Swiatlo");
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO);
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	//UstawKolorPozycji(ID_MENU_SWIATLA, 0);
	sprintf_s(buf,255, " - otaczajace  ");
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - 10);
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	//UstawKolorPozycji(ID_MENU_SWIATLA, 1);
	sprintf_s(buf,255, " - rozproszone ");
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - 20);
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	// Przywrocenie macierzy sprzed wywolania funkcji
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

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

	RysujNakladke();

	glutSwapBuffers();
}

//////////////////////////////
/*	LOAD POINTS FROM FILE	*/
//////////////////////////////

void insertPoints(char **fileName)
{
	FILE *pointsFile;
	fopen_s(&pointsFile,fileName, "r");
	const size_t line_size = 300;
	char* line = malloc(line_size);
	char separator[] = " ,;";
	char *tmp;
	int u = 0, v = 0;
	i = 0, j = 0;
	char *nextToken = NULL;
	while (fgets(line, line_size, pointsFile) != NULL)  
	{
			tmp = strtok_s(line, separator, &nextToken);
			while (tmp != NULL)
			{
				surfaces[j].pts[u][v][i] = atof(tmp);				
				//printf("pts[%d][%d][%d] : %.2f", u, v, i, pts1[u][v][i]);
				tmp = strtok_s(NULL, separator, &nextToken);
				i++;
				if (i == 3)
				{
					i = 0;
					v++;
					if (v == 4)
					{
						u++;
						v = 0;
						if (u == 4)
						{
							u = 0;
							j++;
						}							
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

void insertWeights(char **fileName)
{
	FILE *weightsFile;
	fopen_s(&weightsFile, fileName, "r");
	const size_t line_size = 300;
	char* line = malloc(line_size);
	int u = 0, v = 0, y = 0;
	i = 0,j = 0,k = 0;
	while (fgets(line, line_size, weightsFile) != NULL)  
	{
		//printf("%.2f \n", atof(line));
		surfaces[j].weights[k] = atof(line);

		for (i = 0; i < 3; i++)			
			surfaces[j].pts[u][v][i] *= surfaces[j].weights[k];

		k++;
		v++;
		if (v == 4)
		{
			u++;
			v = 0;
			if (u == 4)
			{
				u = 0;
				j++;
				k = 0;
			}
				
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
	int u = 0, v = 0, y = 0;
	i = 0, j = 0;
	char *nextToken = NULL;
	while (fgets(line, line_size, KnotsUFile) != NULL)  
	{
		//printf("%.2f \n", atof(line));
		surfaces[j].knotsU[i] = atof(line);
		i++;
		if (i == 8)
		{
			j++;
			i = 0;
		}			
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
	int u = 0, v = 0, y = 0;
	i = 0, j = 0;
	char *nextToken = NULL;
	while (fgets(line, line_size, KnotsVFile) != NULL)  
	{
		surfaces[j].knotsV[i] = atof(line);
		i++;
		if (i == 8)
		{
			j++;
			i = 0;
		}
	}
	free(line);
	fclose(KnotsVFile);
}

//////////////////////
/*	MAIN FUNCTION	*/
//////////////////////

int main(int argc, char *argv[]){

	printf("Number of arguments: %d \n", argc);	
	for (i = 1; i < argc; i++){
		printf("arg%d=%s \n",i,argv[i]);		
	}

	if (argc > 5){
		numberOfSurfaces = atoi(argv[1]);
		if (numberOfSurfaces > 5)
			numberOfSurfaces = 5;
		insertPoints(argv[2]);
		insertWeights(argv[3]);
		insertKnotsU(argv[4]);
		insertKnotsV(argv[5]);
	}
	else
		printf_s("Not enough arguments !\n");

	// TESTOWANIE DANYCH
	//int x = 1;
	/*
	for (i = 0; i < numberOfSurfaces; i++){
		for (j = 0; j < 4; j++){
			for (k = 0; k < 4; k++){
				printf("%d %.2f %.2f %.2f \n",x, surfaces[i].pts[j][k][0], surfaces[i].pts[j][k][1], surfaces[i].pts[j][k][2]);
				x++;
			}
		}
	}
	*/
	/*
	for (i = 0; i < numberOfSurfaces; i++){
		for (j = 0; j < 16; j++){
				printf("%d %.2f \n", x, surfaces[i].weights[j]);
				x++;
		}
	}
	*/
	/*
	for (i = 0; i < numberOfSurfaces; i++){
		for (j = 0; j < 8; j++){
			printf("%d %.2f \n", x, surfaces[i].knotsV[j]);
			x++;
		}
	}
	*/
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