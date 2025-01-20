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

#define MIN_PLAY_TIME 20 //minimalny odstêp czasu miêdzy obliczaniem kolejnej planszy

enum MENU {		//na którym ekranie jesteœmy
	BOARD,	//gra
	MAIN	//menu g³ówne
};

class GameInstance
{
	sf::Clock zegar;	//obiekt z zegarem (do gry)
	bool czyGraDziala = false;	//czy gra jest w³¹czona (obliczanie komórek)
	int odstepCzasu = 200;		//odstêp miêdzy obliczeniami w milisekundach (default 200)

	PatternContainer wzorki;
	int windowWidth = 640;
	int windowHeight = 480; //pocz¹tkowy rozmiar okna
	sf::RenderWindow *window;
	MENU menu = MAIN;	//zmienna do wyboru ekranu (startujemy na g³ównym menu)
	const sf::Color background = sf::Color(100, 100, 100, 150); //kolor t³a

	Mapa *cellMap;
	MainMenu *mainMenu;	//klasa menu g³ównego
	GameMenu *gameMenu;	//klasa z kontrolkami do gry
	TileMap *board;
public:
	GameInstance();
	~GameInstance();
	void handleEvents();

};

