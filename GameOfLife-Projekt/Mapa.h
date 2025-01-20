#pragma once
class Mapa
{
	int decideCellState(int neighbourCnt, int currentState);
	void calculateNeighbours(int cellNum);	//oblicz s¹siednie komórki
	int countNeighbours(int cellNum);		//policz ¿ywych s¹siadów komórki
	
	int* generateNeighbourNums(int cellNum);

public:
	int* map;
	int width, height;
	bool zawijanie;
	void calculateMapa();	//oblicz nastêpn¹ turê
	void resetMapa(); // wyzeruj mapê
	void resizePlansza(int deltaWidth, int deltaHeight);	//zmieñ rozmiar mapy
	Mapa();
	Mapa(int width, int height);
	~Mapa();
};

