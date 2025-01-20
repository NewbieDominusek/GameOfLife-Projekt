#pragma once
class Mapa
{
	int decideCellState(int neighbourCnt, int currentState);
	void calculateNeighbours(int cellNum);	//oblicz s�siednie kom�rki
	int countNeighbours(int cellNum);		//policz �ywych s�siad�w kom�rki
	
	int* generateNeighbourNums(int cellNum);

public:
	int* map;
	int width, height;
	bool zawijanie;
	void calculateMapa();	//oblicz nast�pn� tur�
	void resetMapa(); // wyzeruj map�
	void resizePlansza(int deltaWidth, int deltaHeight);	//zmie� rozmiar mapy
	Mapa();
	Mapa(int width, int height);
	~Mapa();
};

