#include <stdio.h>
#include <stdlib.h>
#include <gl\glut.h>

GLfloat knots[8] = { 0.0, 0.0, 0.0, 0.0, 0.8, 0.8, 0.8, 0.8 };
GLfloat knots1[8] = { 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0 };
GLfloat pts1[4][4][3], pts2[4][4][3];
GLfloat pts3[4][4][3], pts4[4][4][3];
GLUnurbsObj *nurb;
int u, v;

#define DLUGOSC_BOKU            5.0
#define OBSERWATOR_FOV_Y        30.0  
#define M_PI 3.1415926535
#define DEG2RAD(x) ((float)(x)*M_PI/180.0)

/////////////////////////////////////////////////////////////////////
// OGRANICZENIE
// Odleg³oœæ
double max_odl = 100;
double min_odl = 10;

// Podzia³
double max_N = 64;
double min_N = 4;

// Promieñ wewnêtrzny
double max_Rw = 10;
double min_Rw = 0;

// D³ugoœæ boku

/////////////////////////////////////////////////////////////////////

// Zmienne globalne
double  bok = DLUGOSC_BOKU; // Dlugosc boku szescianu
int     szerokoscOkna = 800;
int     wysokoscOkna = 600;

// Liczba podzia³ów
double N = 20.0;

// Zmienne globalne do sto¿ka
double h = 5.0;
double r = 3.0;
int tmp = 0;

// Zmienne globalne pierœcienia
double Rw = 10; // promieñ wewnêtrzny
double a = 4; // d³ugoœæ ramienia w trójk¹cie prostok¹tnym



GLfloat odleglosc = 50.0;
GLfloat rotObsZ = 0.0;
GLfloat rotObsY = 20.0;
GLfloat rotObsX = 20.0;


static void
display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCallList(1);
	glFlush();
}

void rysuj_osie()
{
	// Pocztaek tworzenia ukladu wspolrzednych
	glLineWidth(1.0f);
	glBegin(GL_LINES);

	// Os X
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-40.0, 0.0, 0.0);
	glVertex3f(60.0, 0.0, 0.0);

	// Os Y
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, -20.0, 0.0);
	glVertex3f(0.0, 20.0, 0.0);

	// Os Z
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, -20.0);
	glVertex3f(0.0, 0.0, 50.0);

	// Koniec tworzenia ukladu wspolrzednych
	glEnd();
}

void UstawParametryWidoku(int szer, int wys)
{
	// Zapamietanie wielkosci widoku
	szerokoscOkna = szer;
	wysokoscOkna = wys;

	// Ustawienie parametrow viewportu
	glViewport(0, 0, szerokoscOkna, wysokoscOkna);

	// Przejscie w tryb modyfikacji macierzy rzutowania
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(OBSERWATOR_FOV_Y, (float)szerokoscOkna / (float)wysokoscOkna, 1.0, 1000.0);
}

void rysujNurbs(){

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
			//pts4[u][v][3] = 0;
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
	//rysuj_osie();


	gluBeginSurface(nurb);
	gluNurbsSurface(nurb, 8, knots, 8, knots,
		4 * 3, 3, &pts1[0][0][0],
		4, 4, GL_MAP2_VERTEX_3);
	gluEndSurface(nurb);
	/*

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
	*/

}

void ObslugaKlawiszySpecjalnych(int klawisz, int x, int y)
{
	switch (klawisz)
	{
	case GLUT_KEY_UP:
		if (rotObsX < 360) rotObsX += 1.0;
		else rotObsX = 0;
		break;

	case GLUT_KEY_DOWN:
		if (rotObsX > 0) rotObsX -= 1.0;
		else rotObsX = 360;
		break;

	case GLUT_KEY_LEFT:
		if (rotObsY > 0) rotObsY -= 1.0;
		else rotObsY = 360;
		break;

	case GLUT_KEY_RIGHT:
		if (rotObsY < 360) rotObsY += 1.0;
		else rotObsY = 0;
		break;
	}


}

void ObslugaKlawiatury(unsigned char klawisz, int x, int y)
{
	switch (klawisz)
	{
	case '+':
		bok *= 2.0;
		break;

	case '-':
		bok /= 2.0;
		break;

	case ',':
		if (rotObsZ < 360) rotObsZ += 1.0;
		else rotObsZ = 0;
		break;

	case '.':
		if (rotObsZ > 0) rotObsZ -= 1.0;
		else rotObsZ = 360;
		break;

		// odleg³oœæ
	case '[':
		odleglosc = (odleglosc < max_odl) ? odleglosc + 1.0 : odleglosc;
		break;

	case ']':
		odleglosc = (odleglosc > min_odl) ? odleglosc - 1.0 : odleglosc;
		break;

		// podzia³
	case 'q':
		N = (N > min_N) ? N - 1.0 : N;
		break;

	case 'w':
		N = (N < max_N) ? N + 1.0 : N;
		break;

		// promieñ wewnêtrzny
	case 'a':
		Rw = (Rw > min_Rw) ? Rw - 1.0 : Rw;
		break;

	case 's':
		Rw = (Rw < max_Rw) ? Rw + 1.0 : Rw;
		break;

	case 'z':
		a = (a > 0.0) ? a - 1.0 : a;
		break;

	case 'x':
		a = (a < 4.0) ? a + 1.0 : a;
		break;
	}

	if (klawisz == 27)
		exit(0);

}

void WyswietlObraz(void)
{
	// Wyczyszczenie bufora koloru i bufora glebokosci
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Przejscie w tryb modyfikacji macierzy przeksztalcen geometrycznych
	glMatrixMode(GL_MODELVIEW);

	// Zastapienie aktywnej macierzy macierza jednostkowa
	glLoadIdentity();

	// Ustalenie polozenia obserwatora
	glTranslatef(0, 0, -odleglosc);
	glRotatef(rotObsX, 1, 0, 0);
	glRotatef(rotObsY, 0, 1, 0);
	glRotatef(rotObsZ, 0, 0, 1);


	// Narysowanie oœ
	rysuj_osie();

	// Narysowanie bry³
	//RysujSzescian(bok);
	//RysujStozek(h, r);
	//RysujPierscien(Rw, a);
	rysujNurbs();
	// Przelaczenie buforow ramki
	glutSwapBuffers();
}

int main(int argc, char **argv){
	/*
	glutInit(&argc, argv);
	glutInitWindowSize(800, 600);
	glutCreateWindow("NURBS");
	//glutCreateSubWindow(win,100,100,600,400);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	//glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	//glEnable(GL_LIGHTING);
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
			//pts4[u][v][3] = 0;
		}
	}
	
	pts1[3][3][2] = 6;
	
	pts2[0][0][2] = -2;
	
	pts1[0][0][2] = 1;
	pts2[3][3][2] = 1;
	pts3[3][0][2] = 1;
	pts4[0][3][2] = 1;

	glMatrixMode(GL_PROJECTION);
	gluPerspective(55.0, 1.0, 2.0, 24.0);
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(0.0, 0.0, -15.0);
	glRotatef(330.0, 1.0, 0.0, 0.0);
	rysuj_osie();


	glNewList(1, GL_COMPILE);
	
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
	glEndList();
	
	glutDisplayFunc(display);
	glutMainLoop();*/

// Zainicjowanie biblioteki GLUT
glutInit(&argc, argv);

// Ustawienie trybu wyswietlania
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

// Ustawienie polozenia dolenego lewego rogu okna
glutInitWindowPosition(100, 100);

// Ustawienie rozmiarow okna
glutInitWindowSize(szerokoscOkna, wysokoscOkna);

// Utworzenie okna
glutCreateWindow("Szescian");

// Odblokowanie bufora glebokosci
glEnable(GL_DEPTH_TEST);

// Ustawienie wartosci czyszczacej zawartosc bufora glebokosci
glClearDepth(1000.0);

// Ustawienie koloru czyszczenia bufora ramki
glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

// Wlaczenie wyswietlania wielokatow w postaci obrysow (przydatne w celach diagnostycznych).
glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

// Zarejestrowanie funkcji (callback) odpowiedzialnej za 
glutDisplayFunc(WyswietlObraz);

// Zarejestrowanie funkcji (callback) wywolywanej za kazdym razem kiedy
// zmieniane sa rozmiary okna
glutReshapeFunc(UstawParametryWidoku);

// Zarejestrowanie funkcji wykonywanej gdy okno nie obsluguje
// zadnych zadan
glutIdleFunc(WyswietlObraz);

// Zarejestrowanie funkcji obslugi klawiatury
glutKeyboardFunc(ObslugaKlawiatury);

// Zarejestrowanie funkcji obslugi klawiszy specjalnych
glutSpecialFunc(ObslugaKlawiszySpecjalnych);

// Obsluga glownej petli programu (wywolywanie zarejestrowanych callbackow
// w odpowiedzi na odbierane zdarzenia lub obsluga stanu bezczynnosci)
glutMainLoop();

	return 0;
}