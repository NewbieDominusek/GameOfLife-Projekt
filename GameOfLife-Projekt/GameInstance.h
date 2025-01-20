#pragma once
#include <SFML/Graphics.hpp>
#include "kwadraty.h"
#include "MainMenu.h"
#include "GameMenu.h"
#include "PatternContainer.h"
#include "Mapa.h"

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <windows.h>
#include <shellapi.h>

#define MIN_PLAY_TIME 20 //minimalny odst�p czasu mi�dzy obliczaniem kolejnej planszy

enum MENU {		//na kt�rym ekranie jeste�my
	BOARD,	//gra
	MAIN	//menu g��wne
};

class GameInstance
{
	sf::Clock zegar;	//obiekt z zegarem (do gry)
	bool czyGraDziala = false;	//czy gra jest w��czona (obliczanie kom�rek)
	int odstepCzasu = 200;		//odst�p mi�dzy obliczeniami w milisekundach (default 200)

	PatternContainer wzorki;
	int windowWidth = 640;
	int windowHeight = 480; //pocz�tkowy rozmiar okna
	sf::RenderWindow *window;
	MENU menu = MAIN;	//zmienna do wyboru ekranu (startujemy na g��wnym menu)
	const sf::Color background = sf::Color(100, 100, 100, 150); //kolor t�a

	Mapa *cellMap;
	MainMenu *mainMenu;	//klasa menu g��wnego
	GameMenu *gameMenu;	//klasa z kontrolkami do gry
	TileMap *board;
public:
	GameInstance();
	~GameInstance();
	void handleEvents();

};

