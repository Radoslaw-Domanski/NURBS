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
#define Y_OFFSET_SWIATLO    700
const int stepValue = 20;

char buf[255];
GLfloat Rtlo = 0.00f;
GLfloat Gtlo = 0.00f;
GLfloat Btlo = 0.00f;
float Robiekt = 1.00f;
float Gobiekt = 1.00f;
float Bobiekt = 1.00f;
float SamplingTolerance = 10.0;
int UStep = 100;
int VStep = 100;
int valCulling = 0;
char *Culling = "FALSE";
int valAutoLoadMatrix = 1;
char *AutoLoadMatrix = "TRUE";
int valDisplayMode = 0;
char *DisplayMode = "GLU_FILL";
int valSamplingMethod = 0;
char *SamplingMethod = "GLU_PATH_LENGTH";
float ParametricTolerance = 0.5f;
char axes = '1';
char menu = '1';
char obiekt = '1';
char tlo = '0';
int ogolne = 0;
int podswietlenie = 0;
int podswietlenie2 = 0;
double maxDepth = 1000;
double minDepth = 1;
struct Surface surfaces[5];
int windowWidth = 1024;
int windowHeight = 768;
int	numberOfSurfaces = 0;
int currentSurfaces = 0;

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

	glViewport((float)(width/height), 0, windowWidth, windowHeight);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(OBSERWATOR_FOV_Y, (float)windowWidth / (float)windowHeight, 1.0, 1000.0);
	
	/*
	glMatrixMode(GL_PROJECTION);
	windowWidth = width;
	windowHeight = height;
	glLoadIdentity();
	aspect = (float)(width / height);
	int w = height * aspect;           // w is width adjusted for aspect ratio
	int left = (width - w) / 2;
	glViewport(left, 0, w, height);       // fix up the viewport to maintain aspect ratio
	gluOrtho2D(0, windowWidth, windowHeight, 0);   // only the window is changing, not the camera
	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
	*/
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
	glClearColor(Rtlo,Gtlo,Btlo, 0.0f);
	glColor3f(Robiekt,Gobiekt,Bobiekt);
	//glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	nurb = gluNewNurbsRenderer();
	// SAMPLING TOLERANCE
	gluNurbsProperty(nurb, GLU_SAMPLING_TOLERANCE, SamplingTolerance);
	// DISPLAY MODE
	if (valDisplayMode == 0)
		gluNurbsProperty(nurb, GLU_DISPLAY_MODE, GLU_FILL);	
	else if (valDisplayMode == 1)
		gluNurbsProperty(nurb, GLU_DISPLAY_MODE, GLU_OUTLINE_POLYGON);
	else if (valDisplayMode == 2)
		gluNurbsProperty(nurb, GLU_DISPLAY_MODE, GLU_OUTLINE_PATCH);
	// SAMPLING METHOD
	if (valSamplingMethod == 0)
		gluNurbsProperty(nurb, GLU_SAMPLING_METHOD, GLU_PATH_LENGTH);
	else if (valSamplingMethod == 1)
		gluNurbsProperty(nurb, GLU_SAMPLING_METHOD, GLU_PARAMETRIC_ERROR);
	else if (valSamplingMethod == 2)
		gluNurbsProperty(nurb, GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);
	// PARAMETRIC TOLERANCE
	gluNurbsProperty(nurb, GLU_PARAMETRIC_TOLERANCE, ParametricTolerance);
	// U STEP
	gluNurbsProperty(nurb, GLU_U_STEP, UStep);
	// V STEP
	gluNurbsProperty(nurb, GLU_V_STEP, VStep);
	// CULLING
	if (valCulling == 1)
		gluNurbsProperty(nurb, GLU_CULLING, GL_TRUE);
	else
		gluNurbsProperty(nurb, GLU_CULLING, GL_FALSE);
	// AUTO LAOD MATRIX
	if (valAutoLoadMatrix == 1)
		gluNurbsProperty(nurb, GLU_AUTO_LOAD_MATRIX, GL_TRUE);
	else
		gluNurbsProperty(nurb, GLU_AUTO_LOAD_MATRIX, GL_FALSE);

	//gluNurbsProperty(nurb, GLU_NURBS_MODE, GLU_OUTLINE_PATCH);
	//gluNurbsProperty(nurb, GLU_AUTO, GLU_PARAMETRIC_ERROR);

	glMatrixMode(GL_PROJECTION);
	glMatrixMode(GL_MODELVIEW);

	for (i = 0; i < currentSurfaces; i++)
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

	case GLUT_KEY_F1:
		if (menu == '1')
			menu = '0';
		else menu = '1';
		break;
	
	case GLUT_KEY_F2:
		if (axes == '1')
			axes = '0';
		else axes = '1';
		break;

	case GLUT_KEY_F3:
		ogolne++;
		if (ogolne == 3)
			ogolne = 0;
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

	case 'k':
		if (ogolne == 1)
		{
			if (podswietlenie < 9)
				podswietlenie++;
		}
		else if (ogolne == 2){
			if (podswietlenie2 < 15)
				podswietlenie2++;
		}
		break;

	case 'i':
		if (podswietlenie > 0)
			podswietlenie--;
		if (ogolne == 1)
		{
			if (podswietlenie > 0)
				podswietlenie--;
		}
		else if (ogolne == 2){
			if (podswietlenie2 > 0)
				podswietlenie2--;
		}
		break;

	case 'l':
		if (ogolne == 1)
		{
			if (podswietlenie == 1)
			{
				if (Rtlo < 1.0f)
				{
					Rtlo += 0.01f;
					if (Rtlo > 1.0f)
						Rtlo = 1.0f;
				}
			}
			else if (podswietlenie == 2)
			{
				if (Gtlo < 1.0f)
				{
					Gtlo += 0.01f;
					if (Gtlo > 1.0f)
						Gtlo = 1.0f;
				}
			}
			else if (podswietlenie == 3)
			{
				if (Btlo < 1.0f)
				{
					Btlo += 0.01f;
					if (Btlo > 1.0f)
						Btlo = 1.0f;
				}
			}
			else if (podswietlenie == 5)
			{
				if (Robiekt < 1.0f)
				{
					Robiekt += 0.01f;
					if (Robiekt > 1.0f)
						Robiekt = 1.0f;
				}
			}
			else if (podswietlenie == 6)
			{
				if (Gobiekt < 1.0f)
				{
					Gobiekt += 0.01f;
					if (Gobiekt > 1.0f)
						Gobiekt = 1.0f;
				}
			}
			else if (podswietlenie == 7)
			{
				if (Bobiekt < 1.0f)
				{
					Bobiekt += 0.01f;
					if (Bobiekt > 1.0f)
						Bobiekt = 1.0f;
				}
			}
			else if (podswietlenie == 9)
			{
				if (currentSurfaces < numberOfSurfaces)
				{
					currentSurfaces += 1;					
				}
			}
			// AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
			/*
			else if (podswietlenie == 11)
			{
				if (valDisplayMode == 0)
				{
					valDisplayMode++;
					DisplayMode = "GLU_OUTLINE_POLYGON";
				}
				else if (valDisplayMode == 1)
				{
					valDisplayMode++;
					DisplayMode = "GLU_OUTLINE_PATCH";
				}
				else if (valDisplayMode == 2){
					valDisplayMode = 0;
					DisplayMode = "GLU_FILL";
				}
			}*/

		}
		else if (ogolne == 2)
		{
			if (podswietlenie2 == 1)
			{
				valSamplingMethod++;
				if (valSamplingMethod == 1)
					SamplingMethod = "GLU_PARAMETRIC_ERROR";
				else if (valSamplingMethod == 2)
					SamplingMethod = "GLU_DOMAIN_DISTANCE";
				else
				{
					valSamplingMethod = 0;
					SamplingMethod = "GLU_PATH_LENGTH";
				}
			}
			else if (podswietlenie2 == 3)
			{
				if (SamplingTolerance < 50.00f)
					SamplingTolerance += 1.00f;
				else
					SamplingTolerance = 50.00f;
			}
			else if (podswietlenie2 == 5)
			{
				if (ParametricTolerance < 1.00f)
					ParametricTolerance += 0.01f;
				else
					ParametricTolerance = 1.00f;
			}
			else if (podswietlenie2 == 7)
			{
				if (UStep < 200)
					UStep += 1;
				else
					UStep = 200;
			}
			else if (podswietlenie2 == 9)
			{
				if (VStep < 200)
					VStep += 1;
				else
					VStep = 200;
			}
			else if (podswietlenie2 == 11)
			{
				if (valCulling == 1)
				{
					valCulling = 0;
					Culling = "FALSE";
				}					
				else
				{
					valCulling = 1;
					Culling = "TRUE";
				}
					
			}
			else if (podswietlenie2 == 13)
			{
				if (valAutoLoadMatrix == 1)
				{
					valAutoLoadMatrix = 0;
					AutoLoadMatrix = "FALSE";
				}
					
				else
				{					
					valAutoLoadMatrix = 1;
					AutoLoadMatrix = "TRUE";
				}
					
			}
			else if (podswietlenie2 == 15)
			{
				if (valDisplayMode == 0)
				{
					valDisplayMode++;
					DisplayMode = "GLU_OUTLINE_POLYGON";
				}
				else if (valDisplayMode == 1)
				{
					valDisplayMode++;
					DisplayMode = "GLU_OUTLINE_PATCH";
				}
				else if (valDisplayMode == 2){
					valDisplayMode = 0;
					DisplayMode = "GLU_FILL";
				}
			}
		}
		break;

	case 'j':
		if (ogolne == 1)
		{
			if (podswietlenie == 1)
			{
				if (Rtlo > 0.00f)
				{
					Rtlo -= 0.01f;
					if (Rtlo < 0.00f)
						Rtlo = 0.00f;
				}
			}
			else if (podswietlenie == 2)
			{
				if (Gtlo > 0.00f)
				{
					Gtlo -= 0.01f;
					if (Gtlo < 0.00f)
						Gtlo = 0.00f;
				}
			}
			else if (podswietlenie == 3)
			{
				if (Btlo > 0.00f)
				{
					Btlo -= 0.01f;
					if (Btlo < 0.00f)
						Btlo = 0.00f;
				}
			}
			else if (podswietlenie == 5)
			{
				if (Robiekt > 0.00f)
				{
					Robiekt -= 0.01f;
					if (Robiekt < 0.00f)
						Robiekt = 0.00f;
				}
			}
			else if (podswietlenie == 6)
			{
				if (Gobiekt > 0.00f)
				{
					Gobiekt -= 0.01f;
					if (Gobiekt < 0.00f)
						Gobiekt = 0.00f;
				}
			}
			else if (podswietlenie == 7)
			{
				if (Bobiekt > 0.00f)
				{
					Bobiekt -= 0.01f;
					if (Bobiekt < 0.00f)
						Bobiekt = 0.00f;
				}
			}
			else if (podswietlenie == 9)
			{
				if (currentSurfaces > 0)
				{
					currentSurfaces -= 1;					
				}
			}
			// AAAAAAAAAAAAAAAAAAAAAAAAAA
			/*
			else if (podswietlenie == 11)
			{
				if (valDisplayMode == 0)
				{
					valDisplayMode = 2;
					DisplayMode = "GLU_OUTLINE_PATCH";
				}
				else if (valDisplayMode == 1)
				{
					valDisplayMode--;
					DisplayMode = "GLU_FILL";
				}
				else if (valDisplayMode == 2){
					valDisplayMode--;
					DisplayMode = "GLU_OUTLINE_POLYGON";
				}
			}*/

		}
		else if (ogolne == 2)
		{
			if (podswietlenie2 == 1)
			{
				valSamplingMethod--;
				if (valSamplingMethod == 1)				
					SamplingMethod = "GLU_PARAMETRIC_ERROR";				
				else if (valSamplingMethod == 0)
					SamplingMethod = "GLU_PATH_LENGTH";
				else
				{
					SamplingMethod = "GLU_DOMAIN_DISTANCE";
					valSamplingMethod = 2;
				}					
			}
			else if (podswietlenie2 == 3)
			{
				if (SamplingTolerance > 1.00f)
					SamplingTolerance -= 1.00f;
				else
					SamplingTolerance = 1.00f;
			}
			else if (podswietlenie2 == 5)
			{
				if (ParametricTolerance > 0.02f)
					ParametricTolerance -= 0.01f;
				else
					ParametricTolerance = 0.01f;
			}
			else if (podswietlenie2 == 7)
			{
				if (UStep > 2)
					UStep -= 1;
				else
					UStep = 1;
			}
			else if (podswietlenie2 == 9)
			{
				if (VStep > 2)
					VStep -= 1;
				else
					VStep = 1;
			}
			else if (podswietlenie2 == 11)
			{
				if (valCulling == 1)
				{
					valCulling = 0;
					Culling = "FALSE";
				}
				else
				{
					valCulling = 1;
					Culling = "TRUE";
				}
					
			}
			else if (podswietlenie2 == 13)
			{
				if (valAutoLoadMatrix == 1)
				{
					valAutoLoadMatrix = 0;
					AutoLoadMatrix = "FALSE";
				}					
				else
				{
					AutoLoadMatrix = "TRUE";
					valAutoLoadMatrix = 1;
				}
					
			}
			else if (podswietlenie2 == 15)
			{
				if (valDisplayMode == 0)
				{
					valDisplayMode = 2;
					DisplayMode = "GLU_OUTLINE_PATCH";
				}
				else if (valDisplayMode == 1)
				{
					valDisplayMode--;
					DisplayMode = "GLU_FILL";
				}
				else if (valDisplayMode == 2){
					valDisplayMode--;
					DisplayMode = "GLU_OUTLINE_POLYGON";
				}
			}
		}
		break;
	}
	// ESC
	if (key == 27)
		exit(0);

}

void RysujTekstRastrowy(void *font, char *tekst)
{
	for (i = 0; i < (int)strlen(tekst); i++)
		glutBitmapCharacter(font, tekst[i]);
}

void IfChoosen(int x){
	if (podswietlenie == x)
		glColor3f(1.0, 0.0, 0.0);
	else glColor3f(1.0, 1.0, 1.0);
}

void IfChoosen2(int x){
	if (podswietlenie2 == x)
		glColor3f(1.0, 0.0, 0.0);
	else glColor3f(1.0, 1.0, 1.0);
}

void DrawOgolne(){	
	int step = 1;
	
	sprintf_s(buf, 255, "F1  - widocznosc menu");
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	sprintf_s(buf, 255, "F2  - widocznosc osi");
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	sprintf_s(buf, 255, "F3  - zmiana menu");
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	sprintf_s(buf, 255, "+/- - przyblizenie");
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	sprintf_s(buf, 255, "GORA/DOL - obrot wzgledem osi X");
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	sprintf_s(buf, 255, "LEWO/PRAWO - obrot wzgledem osi Y");
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	sprintf_s(buf, 255, "GWIAZDKA/SLASH - obrot wzgledem osi Z");
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	sprintf_s(buf, 255, "I/K/J/L - nawigacja w menu");
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

}

void DrawTlo(){
	int step = 1;

	IfChoosen(0);
	sprintf_s(buf, 255, "Kolor tla");
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	IfChoosen(1);
	sprintf_s(buf, 255, "    R = %.2f", Rtlo);
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	IfChoosen(2);
	sprintf_s(buf, 255, "    G = %.2f", Gtlo);
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	IfChoosen(3);
	sprintf_s(buf, 255, "    B = %.2f", Btlo);
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	IfChoosen(4);
	sprintf_s(buf, 255, "Kolor obiektu");
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	IfChoosen(5);
	sprintf_s(buf, 255, "    R = %.2f", Robiekt);
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	IfChoosen(6);
	sprintf_s(buf, 255, "    G = %.2f", Gobiekt);
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	IfChoosen(7);
	sprintf_s(buf, 255, "    B = %.2f", Bobiekt);
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	IfChoosen(8);
	sprintf_s(buf, 255, "Liczba modelowanych powierzchni");
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	IfChoosen(9);
	sprintf_s(buf, 255, "    %d(%d)", currentSurfaces, numberOfSurfaces);
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

}


void DrawSzczegolowe(){
	int step = 1;

	IfChoosen2(0);
	sprintf_s(buf, 255, "GLU_SAMPLING_METHOD");
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	IfChoosen2(1);
	sprintf_s(buf, 255, "    %s", SamplingMethod);
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	IfChoosen2(2);
	sprintf_s(buf, 255, "GLU_SAMPLING_TOLERANCE");
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	IfChoosen2(3);
	sprintf_s(buf, 255, "    %.2f", SamplingTolerance);
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	IfChoosen2(4);
	sprintf_s(buf, 255, "GLU_PARAMETRIC_TOLERANCE");
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	IfChoosen2(5);
	sprintf_s(buf, 255, "    %.2f",ParametricTolerance);
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	IfChoosen2(6);
	sprintf_s(buf, 255, "GLU_U_STEP");
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	IfChoosen2(7);
	sprintf_s(buf, 255, "    %d", UStep);
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	IfChoosen2(8);
	sprintf_s(buf, 255, "GLU_V_STEP");
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	IfChoosen2(9);
	sprintf_s(buf, 255, "    %d", VStep);
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	IfChoosen2(10);
	sprintf_s(buf, 255, "GLU_CULLING");
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	IfChoosen2(11);
	sprintf_s(buf, 255, "    %s", Culling);
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	IfChoosen2(12);
	sprintf_s(buf, 255, "GLU_AUTO_LOAD_MATRIX");
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	IfChoosen2(13);
	sprintf_s(buf, 255, "    %s", AutoLoadMatrix);
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	IfChoosen2(14);
	sprintf_s(buf, 255, "GLU_DISPLAY_MODE");
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

	IfChoosen2(15);
	sprintf_s(buf, 255, "    %s", DisplayMode);
	glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - (step++*stepValue));
	RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

}

void RysujNakladke(void)
{

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

	if (ogolne == 1)
		DrawTlo();
	else if (ogolne == 2)
		DrawSzczegolowe();
	else
		DrawOgolne();
	
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

	if (axes == '1')
		DrawAxis();
	
	DrawNurbs();

	if (menu == '1')
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
	currentSurfaces = numberOfSurfaces;
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

	glEnable(GL_DEPTH_TEST);

	glClearDepth(1000.0);

	glClearColor(Rtlo, Gtlo, Btlo, 0.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glutDisplayFunc(Display);

	glutReshapeFunc(WindowResize);

	glutIdleFunc(Display);

	glutKeyboardFunc(KeyboardKeys);

	glutSpecialFunc(KeyboardSpecialKeys);

	glutMainLoop();

	return 0;
}