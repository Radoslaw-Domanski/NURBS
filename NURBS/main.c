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

#define CAMERA_ANGLE    30.0  
#define TEXT_POSITION_X    10
#define TEXT_POSITION_Y    700
const int stepValue = 20;

char buffer[255];
GLfloat RBackground = 0.00f;
GLfloat GBackground = 0.00f;
GLfloat BBackground = 0.00f;
float RObject = 1.00f;
float GObject = 1.00f;
float BObject = 1.00f;
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
char object = '1';
char background = '0';
int general = 0;
int highlighted = 0;
int highlighted2 = 0;
double maxDepth = 1000;
double minDepth = 1;
struct Surface surfaces[10];
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

//////////////////////////
/*	DRAW AXIS FUNCTION	*/
//////////////////////////

void DrawAxis()
{
	// linie
	glLineWidth(1.0f);
	glBegin(GL_LINES);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-20.0, 0.0, 0.0);
	glVertex3f(20.0, 0.0, 0.0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, -20.0, 0.0);
	glVertex3f(0.0, 20.0, 0.0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, -20.0);
	glVertex3f(0.0, 0.0, 20.0);
	glEnd();

	// czerwona strzalka
	glColor3f(1.0, 0.0, 0.0);
	glPushMatrix();
	glTranslatef(19.0, 0, 0);
	glRotatef(270, 1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glutSolidCone(0.5, 1.0, 20.0, 20.0);
	glPopMatrix();

	// zielona strzalka
	glColor3f(0.0, 1.0, 0.0);
	glPushMatrix();
	glTranslatef(0, 19.0, 0);
	glRotatef(270, 1.0, 0.0, 0.0);
	glutSolidCone(0.5, 1.0, 20.0, 20.0);
	glPopMatrix();
	// niebieska strzalka
	glColor3f(0.0, 0.0, 1.0);
	glPushMatrix();
	glTranslatef(0, 0, 19.0);
	glutSolidCone(0.5, 1.0, 20.0, 20.0);
	glPopMatrix();
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
	gluPerspective(CAMERA_ANGLE, (float)windowWidth / (float)windowHeight, 1.0, 1000.0);

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
	glClearColor(RBackground,GBackground,BBackground, 0.0f);
	glColor3f(RObject,GObject,BObject);
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
		general++;
		if (general == 3)
			general = 0;
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
		if (general == 1)
		{
			if (highlighted < 9)
				highlighted++;
		}
		else if (general == 2){
			if (highlighted2 < 15)
				highlighted2++;
		}
		break;

	case 'i':		
		if (general == 1)
		{
			if (highlighted > 0)
				highlighted--;
		}
		else if (general == 2){
			if (highlighted2 > 0)
				highlighted2--;
		}
		break;

	case 'l':
		if (general == 1)
		{
			if (highlighted == 1)
			{
				if (RBackground < 1.0f)
				{
					RBackground += 0.01f;
					if (RBackground > 1.0f)
						RBackground = 1.0f;
				}
			}
			else if (highlighted == 2)
			{
				if (GBackground < 1.0f)
				{
					GBackground += 0.01f;
					if (GBackground > 1.0f)
						GBackground = 1.0f;
				}
			}
			else if (highlighted == 3)
			{
				if (BBackground < 1.0f)
				{
					BBackground += 0.01f;
					if (BBackground > 1.0f)
						BBackground = 1.0f;
				}
			}
			else if (highlighted == 5)
			{
				if (RObject < 1.0f)
				{
					RObject += 0.01f;
					if (RObject > 1.0f)
						RObject = 1.0f;
				}
			}
			else if (highlighted == 6)
			{
				if (GObject < 1.0f)
				{
					GObject += 0.01f;
					if (GObject > 1.0f)
						GObject = 1.0f;
				}
			}
			else if (highlighted == 7)
			{
				if (BObject < 1.0f)
				{
					BObject += 0.01f;
					if (BObject > 1.0f)
						BObject = 1.0f;
				}
			}
			else if (highlighted == 9)
			{
				if (currentSurfaces < numberOfSurfaces)
				{
					currentSurfaces += 1;					
				}
			}
		}
		else if (general == 2)
		{
			if (highlighted2 == 1)
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
			else if (highlighted2 == 3)
			{
				if (SamplingTolerance < 50.00f)
					SamplingTolerance += 1.00f;
				else
					SamplingTolerance = 50.00f;
			}
			else if (highlighted2 == 5)
			{
				if (ParametricTolerance < 1.00f)
					ParametricTolerance += 0.01f;
				else
					ParametricTolerance = 1.00f;
			}
			else if (highlighted2 == 7)
			{
				if (UStep < 200)
					UStep += 1;
				else
					UStep = 200;
			}
			else if (highlighted2 == 9)
			{
				if (VStep < 200)
					VStep += 1;
				else
					VStep = 200;
			}
			else if (highlighted2 == 11)
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
			else if (highlighted2 == 13)
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
			else if (highlighted2 == 15)
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
		if (general == 1)
		{
			if (highlighted == 1)
			{
				if (RBackground > 0.00f)
				{
					RBackground -= 0.01f;
					if (RBackground < 0.00f)
						RBackground = 0.00f;
				}
			}
			else if (highlighted == 2)
			{
				if (GBackground > 0.00f)
				{
					GBackground -= 0.01f;
					if (GBackground < 0.00f)
						GBackground = 0.00f;
				}
			}
			else if (highlighted == 3)
			{
				if (BBackground > 0.00f)
				{
					BBackground -= 0.01f;
					if (BBackground < 0.00f)
						BBackground = 0.00f;
				}
			}
			else if (highlighted == 5)
			{
				if (RObject > 0.00f)
				{
					RObject -= 0.01f;
					if (RObject < 0.00f)
						RObject = 0.00f;
				}
			}
			else if (highlighted == 6)
			{
				if (GObject > 0.00f)
				{
					GObject -= 0.01f;
					if (GObject < 0.00f)
						GObject = 0.00f;
				}
			}
			else if (highlighted == 7)
			{
				if (BObject > 0.00f)
				{
					BObject -= 0.01f;
					if (BObject < 0.00f)
						BObject = 0.00f;
				}
			}
			else if (highlighted == 9)
			{
				if (currentSurfaces > 0)
				{
					currentSurfaces -= 1;					
				}
			}
		}
		else if (general == 2)
		{
			if (highlighted2 == 1)
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
			else if (highlighted2 == 3)
			{
				if (SamplingTolerance > 1.00f)
					SamplingTolerance -= 1.00f;
				else
					SamplingTolerance = 1.00f;
			}
			else if (highlighted2 == 5)
			{
				if (ParametricTolerance > 0.02f)
					ParametricTolerance -= 0.01f;
				else
					ParametricTolerance = 0.01f;
			}
			else if (highlighted2 == 7)
			{
				if (UStep > 2)
					UStep -= 1;
				else
					UStep = 1;
			}
			else if (highlighted2 == 9)
			{
				if (VStep > 2)
					VStep -= 1;
				else
					VStep = 1;
			}
			else if (highlighted2 == 11)
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
			else if (highlighted2 == 13)
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
			else if (highlighted2 == 15)
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

//////////////////////////
/*	DRAW TEXT IN MENU	*/
//////////////////////////

void DrawText(void *font, char *tekst)
{
	for (i = 0; i < (int)strlen(tekst); i++)
		glutBitmapCharacter(font, tekst[i]);
}

//////////////////////////
/*	HIGHLIGHT MENU TEXT	*/
//////////////////////////

void IfChoosen(int x){
	if (highlighted == x)
		glColor3f(1.0, 0.0, 0.0);
	else glColor3f(1.0, 1.0, 1.0);
}

//////////////////////////////
/*	HIGHLIGHT MENU TEXT 2	*/
//////////////////////////////

void IfChoosen2(int x){
	if (highlighted2 == x)
		glColor3f(1.0, 0.0, 0.0);
	else glColor3f(1.0, 1.0, 1.0);
}

//////////////////////////
/*	DRAW GENERAL MENU	*/
//////////////////////////

void DrawGeneral(){	

	int step = 1;
	
	sprintf_s(buffer, 255, "F1  - widocznosc menu");
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	sprintf_s(buffer, 255, "F2  - widocznosc osi");
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	sprintf_s(buffer, 255, "F3  - zmiana menu");
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	sprintf_s(buffer, 255, "+/- - zmiana przyblizenia");
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	sprintf_s(buffer, 255, "GORA/DOL - obrot wzgledem osi X");
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	sprintf_s(buffer, 255, "LEWO/PRAWO - obrot wzgledem osi Y");
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	sprintf_s(buffer, 255, "GWIAZDKA/SLASH - obrot wzgledem osi Z");
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	sprintf_s(buffer, 255, "I/K  - nawigacja w menu");
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	sprintf_s(buffer, 255, "J/L - zmiana wartosci w menu");
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);
}

//////////////////////
/*	DRAW FIRST MENU	*/
//////////////////////

void DrawBackground(){

	int step = 1;

	IfChoosen(0);
	sprintf_s(buffer, 255, "Kolor tla");
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	IfChoosen(1);
	sprintf_s(buffer, 255, "    R = %.2f", RBackground);
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	IfChoosen(2);
	sprintf_s(buffer, 255, "    G = %.2f", GBackground);
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	IfChoosen(3);
	sprintf_s(buffer, 255, "    B = %.2f", BBackground);
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	IfChoosen(4);
	sprintf_s(buffer, 255, "Kolor objectu");
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	IfChoosen(5);
	sprintf_s(buffer, 255, "    R = %.2f", RObject);
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	IfChoosen(6);
	sprintf_s(buffer, 255, "    G = %.2f", GObject);
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	IfChoosen(7);
	sprintf_s(buffer, 255, "    B = %.2f", BObject);
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	IfChoosen(8);
	sprintf_s(buffer, 255, "Liczba modelowanych powierzchni");
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	IfChoosen(9);
	sprintf_s(buffer, 255, "    %d(%d)", currentSurfaces, numberOfSurfaces);
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

}

//////////////////////////
/*	DRAW SPECIFIC MENU	*/
//////////////////////////

void DrawSpecific(){
	int step = 1;

	IfChoosen2(0);
	sprintf_s(buffer, 255, "GLU_SAMPLING_METHOD");
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	IfChoosen2(1);
	sprintf_s(buffer, 255, "    %s", SamplingMethod);
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	IfChoosen2(2);
	sprintf_s(buffer, 255, "GLU_SAMPLING_TOLERANCE");
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	IfChoosen2(3);
	sprintf_s(buffer, 255, "    %.2f", SamplingTolerance);
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	IfChoosen2(4);
	sprintf_s(buffer, 255, "GLU_PARAMETRIC_TOLERANCE");
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	IfChoosen2(5);
	sprintf_s(buffer, 255, "    %.2f",ParametricTolerance);
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	IfChoosen2(6);
	sprintf_s(buffer, 255, "GLU_U_STEP");
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	IfChoosen2(7);
	sprintf_s(buffer, 255, "    %d", UStep);
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	IfChoosen2(8);
	sprintf_s(buffer, 255, "GLU_V_STEP");
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	IfChoosen2(9);
	sprintf_s(buffer, 255, "    %d", VStep);
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	IfChoosen2(10);
	sprintf_s(buffer, 255, "GLU_CULLING");
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	IfChoosen2(11);
	sprintf_s(buffer, 255, "    %s", Culling);
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	IfChoosen2(12);
	sprintf_s(buffer, 255, "GLU_AUTO_LOAD_MATRIX");
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	IfChoosen2(13);
	sprintf_s(buffer, 255, "    %s", AutoLoadMatrix);
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	IfChoosen2(14);
	sprintf_s(buffer, 255, "GLU_DISPLAY_MODE");
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

	IfChoosen2(15);
	sprintf_s(buffer, 255, "    %s", DisplayMode);
	glRasterPos2i(TEXT_POSITION_X, TEXT_POSITION_Y - (step++*stepValue));
	DrawText(GLUT_BITMAP_8_BY_13, buffer);

}

//////////////////////////
/*	DRAW MENU ON WINDOW	*/
//////////////////////////

void DrawMenu(void)
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

	if (general == 1)
		DrawBackground();
	else if (general == 2)
		DrawSpecific();
	else
		DrawGeneral();
	
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
		DrawMenu();

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
		if (numberOfSurfaces > 10)
			numberOfSurfaces = 10;
		insertPoints(argv[2]);
		insertWeights(argv[3]);
		insertKnotsU(argv[4]);
		insertKnotsV(argv[5]);
	}
	else
		printf_s("Not enough arguments !\n");
	currentSurfaces = numberOfSurfaces;

	char *myargv[1];
	int myargc = 1;
	myargv[0] =_strdup("NURBS");

	// inicjowanie biblioteki
	glutInit(&myargc, myargv);
	// ustawienie trybu wyœwietlania
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	// ustawienie pozycji pocz¹tkowej okna
	glutInitWindowPosition(0, 0);
	// ustawienie pocz¹tkowej szerokoœci i wysokoœci okna
	glutInitWindowSize(windowWidth, windowHeight);
	// stworzenie okna programu o nazwie NURBS
	glutCreateWindow("NURBS");
	// w³¹czenie bufora g³êbokoœci
	glEnable(GL_DEPTH_TEST);
	// ustawienie wartoœci bufora g³êbokoœci
	glClearDepth(1000.0);
	// ustawienie koloru t³a
	glClearColor(RBackground, GBackground, BBackground, 0.0f);
	// ustawienie wyœwietlania wielok¹tów w trybie linii
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// zdefiniowanie g³ównej funkcji, która wyœwietla obraz
	glutDisplayFunc(Display);
	// zdefiniowanie funkcji wywo³ywanej po zmianie rozmiarów okna
	glutReshapeFunc(WindowResize);
	// zdefiniowanie funkcji w przypadku bezczynnoœci programu
	glutIdleFunc(Display);
	// zdefiniowanie funkcji obs³uguj¹cej klawisze klawiatury
	glutKeyboardFunc(KeyboardKeys);
	// zdefiniowanie funkcji obs³uguj¹cej klawisze specjalne
	glutSpecialFunc(KeyboardSpecialKeys);
	// w³¹czenie zapêtlenia wykonywania siê programu
	glutMainLoop();

	return 0;
}