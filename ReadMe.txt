Przedstawiony program modeluje obiekty 3D za pomoc� powierzchni NURBS.

W celu poprawnego jego dzia�ania musi by� zainstalowana biblioteka GLUT 
(dost�pna do pobrania https://github.com/Radoslaw-Domanski/NURBS/blob/master/glut-3.7.6-bin.zip )

W folderze z programem musi znajdowa� si� plik glut32.dll

Program nale�y uruchamia� poprzez pliki *.bat 

Przyk�adowy poprawny plik bat
"NURBS.exe 9 p.txt weights.txt knotsu.txt knotsv.txt"

NURBS.exe - nazwa wywo�ywanego programu
9 - liczba modelowanych powierzchni NURBS
p.txt - plik zawieraj�cy punkty kontrolne
weights.txt - plik zawieraj�cy wagi punkt�w kontrolnych
knotsu.txt - plik zawieraj�cy warto�ci w�z��w w kierunku U
knotsv.txt - plik zawieraj�cy warto�ci w�z��w w kierunku V

Wsp�rz�dne punkt�w musz� by� oddzielone jednym z poni�szych separator�w
,	[spacja]		;

Pojedyncza powierzchnia sk�ada si� z:
16 punkt�w kontrolnych
16 wag punkt�w kontrolnych
8 warto�ci w�z��w w kierunku U
8 warto�ci w�z��w w kierunku V

Wykonanie
Rados�aw Doma�ski, Wojskowa Akademia Techniczna, 2017