Przedstawiony program modeluje obiekty 3D za pomoc¹ powierzchni NURBS.

W celu poprawnego jego dzia³ania musi byæ zainstalowana biblioteka GLUT 
(dostêpna do pobrania https://github.com/Radoslaw-Domanski/NURBS/blob/master/glut-3.7.6-bin.zip )

W folderze z programem musi znajdowaæ siê plik glut32.dll

Program nale¿y uruchamiaæ poprzez pliki *.bat 

Przyk³adowy poprawny plik bat
"NURBS.exe 9 p.txt weights.txt knotsu.txt knotsv.txt"

NURBS.exe - nazwa wywo³ywanego programu
9 - liczba modelowanych powierzchni NURBS
p.txt - plik zawieraj¹cy punkty kontrolne
weights.txt - plik zawieraj¹cy wagi punktów kontrolnych
knotsu.txt - plik zawieraj¹cy wartoœci wêz³ów w kierunku U
knotsv.txt - plik zawieraj¹cy wartoœci wêz³ów w kierunku V

Wspó³rzêdne punktów musz¹ byæ oddzielone jednym z poni¿szych separatorów
,	[spacja]		;

Pojedyncza powierzchnia sk³ada siê z:
16 punktów kontrolnych
16 wag punktów kontrolnych
8 wartoœci wêz³ów w kierunku U
8 wartoœci wêz³ów w kierunku V

Wykonanie
Rados³aw Domañski, Wojskowa Akademia Techniczna, 2017