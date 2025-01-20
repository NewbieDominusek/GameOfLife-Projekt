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

int main()
{
	//DEFINICJE WZOR�W DO WKLEJENIA Z PLIKU "patterns.txt" PLIK MUSI BY� W FOLDERZE Z .exe
	//format pliku:
	//h:[wysoko��].
	//w:[szeroko��].
	//d:
	//001
	//101
	//011.
	//p:[Nazwa wzorku].

	//zapisujemy dok�adnie z kropkami na ko�cu segmentu danych i dwukropkami.
	//nazwa wzorku, czyli p (pattern) musi by� poprzedzone danymi h d w (nie 3 nie musz� by� w dok�adniej kolejno�ci)
	//ten format mo�na powtarza� dowoln� ilo�� razy aby zapisa� wiele wzork�w
	//podane rozmiary wzorku musz� si� zgadza� z ilo�ci� danych, inaczej wzorek b�dzie zignorowany
	PatternContainer wzorki;
	int wybranyWzorek = 0;

	int windowWidth = 640, windowHeight = 480; //pocz�tkowy rozmiar okna
	sf::RenderWindow window(	//Zr�b okno
		sf::VideoMode(windowWidth, windowHeight),
		"Game Of Life");
	window.setFramerateLimit(120);

	enum MENU {		//na kt�rym ekranie jeste�my
		BOARD,	//gra
		MAIN	//menu g��wne
	};

	MENU menu = MAIN;	//zmienna do wyboru ekranu (startujemy na g��wnym menu)

	const sf::Color background = sf::Color(100, 100, 100, 150); //kolor t�a

	Mapa cellMap(3,3);
	MainMenu mainMenu(windowWidth, windowHeight);	//klasa menu g��wnego
	GameMenu gameMenu(windowWidth, windowHeight, cellMap.width, cellMap.height);	//klasa z kontrolkami do gry
	
	TileMap board(&cellMap, windowWidth, windowHeight); //klasa u�ywana do renderowania kom�rek, renderowanie jest bazowane na tablicy map, nale�y przekaza� tablic�, rozmiar tablicy i rozmiar kom�rek do wyrenderowania

	board.holoData = wzorki.patterns[wybranyWzorek].data;	//pocz�tkowy wzorek (domy�lny glider)
	board.holoWidth = wzorki.patterns[wybranyWzorek].width;
	board.holoHeight = wzorki.patterns[wybranyWzorek].height;
	gameMenu.updateBttnText(5,wzorki.patterns[wybranyWzorek].name);

	board.drawBounds = TileMap::YES;	//czy rysowa� kraw�dzie mi�dzy kom�rkami
	board.kolorLinii = sf::Color(100, 200, 40, 70);	//kolor kraw�dzi
	board.updateLinie();	//narysuj linie
	board.updateKwadraty();		//metoda aktualizuje wygl�d renderowanych kom�rek, nale�y u�y� po ka�dej zmianie tablicy map

	sf::Clock zegar;	//obiekt z zegarem (do gry)
	bool czyGraDziala = false;	//czy gra jest w��czona (obliczanie kom�rek)
	int odstepCzasu = 200;		//odst�p mi�dzy obliczeniami w milisekundach (default 200)


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event)) {	//handlowanie event�w
			switch (event.type) {	//zamkni�cie okna
			case sf::Event::Closed:
				window.close();	//zamknij okno
				break;

			case sf::Event::MouseButtonPressed:	//naci�ni�cie myszki
				if (menu == MAIN) {	//je�li jeste�my w menu g��wnym
					switch (mainMenu.handleEvent(event)) {	//main sprawdza na kt�ry przycisk naci�ni�to i zwraca jego index
					case 0:	//START
						menu = BOARD;	//zacznij gr�
						break;
					case 1:
						ShellExecuteA(NULL, "open", ".", NULL, NULL, SW_SHOWDEFAULT);
						break;

					case 2:	//EXIT
						window.close();
					default:	//na nic nie naci�ni�to (skill issue)
						break;
					}
				}
				else {
					switch (gameMenu.handleEvent(event)) {	//sprawdzamy klikni�ty guzik
					case 0:	//MAIN MENU
						menu = MAIN;	//id� do menu
						czyGraDziala = false; //wy��cz gr�
						break;
					case 1: //Slider body
						odstepCzasu = gameMenu.getSliderVal() * 3; //we� warto�� slidera (szybko�� gry)
						if (odstepCzasu < MIN_PLAY_TIME) odstepCzasu = MIN_PLAY_TIME; //nie mo�e by� za ma�a bo komputer jebnie xd
						break;
					case 2: //slider
						odstepCzasu = gameMenu.getSliderVal() * 3;
						if (odstepCzasu < MIN_PLAY_TIME) odstepCzasu = MIN_PLAY_TIME;
						break;
					case 3: //PLAY
						czyGraDziala = true; //w��cz gr�
						zegar.restart();	//wyzeruj timer
						break;
					case 4: //STOP
						czyGraDziala = false; //wy��cz gr�
						break;

					case 5://PASTE IN (HOLOGRAM)
						board.pasteInEnabled = !board.pasteInEnabled; //prze��cz wklejanie hologramu
						board.showHolo = false; //wy��cz rysowanie hologramu (jeste�my poza plansz�)
						board.hologram(false); //wywo�anie funkcji aby ustawi�a plansz� w odpowiedni stan
						board.updateKwadraty(); //od�wierzenie planszy
						break;


					case 6: //width (zwi�kszanie i zmniejszanie)
						if (event.mouseButton.button == sf::Mouse::Left) cellMap.resizePlansza(1,0); //zwi�ksz plansz�
						else if (event.mouseButton.button == sf::Mouse::Right) cellMap.resizePlansza(-1,0);
						gameMenu.updateBttnText(6,std::to_string(cellMap.width));
						board.resizePlansza();
						break;


					case 7: //height (zwi�kszanie i zmniejszanie)
						if (event.mouseButton.button == sf::Mouse::Left) cellMap.resizePlansza(0, 1);
						else if (event.mouseButton.button == sf::Mouse::Right) cellMap.resizePlansza(0, -1);
						gameMenu.updateBttnText(7,std::to_string(cellMap.height));
						board.resizePlansza();
						break;


					case 8: //RESET
						czyGraDziala = false; //wy��cz gr�
						cellMap.resetMapa();
						board.updateKwadraty();
						break;

					case 9: //RESET
						cellMap.zawijanie = !cellMap.zawijanie;
						gameMenu.updateBttnColor(9, (cellMap.zawijanie) ? sf::Color::Green : sf::Color::Red);
						break;


					default:	//nie naci�ni�to na guzik, wi�c mo�e na plansz� z kom�rkami
						board.HandleEvent(event);	//Je�li jeste�my w grze i nie naci�ni�to guzika, to dajemy event do planszy (ona sobie poradzi)
						break;
					}
					
				}
				
				break;

			case sf::Event::Resized:	//zmieniono rozmiar ekranu, trzeba zaktualizowa� zmienne z aktualnym rozmiarem ekranu (obiekty sobie poradz� ez pz)
				board.HandleEvent(event);
				mainMenu.handleEvent(event);
				gameMenu.handleEvent(event);
				break;

			case sf::Event::MouseWheelScrolled: //przescrollowano k�kiem (rozmiar planszy mo�na zmienia� k�kiem)
				if (menu == BOARD) { //musimy by� na ekranie z gr�
					switch (gameMenu.handleEvent(event)) {	//sprawdzamy na co scrollowano

					case 5:
						wybranyWzorek += event.mouseWheelScroll.delta; //dodajemy scrolla
						if (wybranyWzorek < 0) wybranyWzorek = wzorki.patterns.size() -1; //je�li przechodzimy do ty�u to przechodzimy na pocz�tek
						wybranyWzorek = wybranyWzorek % wzorki.patterns.size(); //zawijamy je�li liczba wi�ksza ni� ilo�� element�w
						
						

						board.holoData = wzorki.patterns[wybranyWzorek].data;	//pocz�tkowy wzorek (domy�lny glider)
						board.holoWidth = wzorki.patterns[wybranyWzorek].width;
						board.holoHeight = wzorki.patterns[wybranyWzorek].height;
						gameMenu.updateBttnText(5, wzorki.patterns[wybranyWzorek].name);
						break;

					case 6: //width (zwi�kszanie i zmniejszanie)
						cellMap.resizePlansza((int)event.mouseWheelScroll.delta, 0);

						gameMenu.updateBttnText(6, std::to_string(cellMap.width));
						board.resizePlansza();
						break;


					case 7: //height (zwi�kszanie i zmniejszanie)
						cellMap.resizePlansza(0, (int)event.mouseWheelScroll.delta);

						gameMenu.updateBttnText(7, std::to_string(cellMap.height));
						board.resizePlansza();
						break;
					}
				}
				break;

			case sf::Event::MouseMoved:
				board.HandleEvent(event);
				break;

			default:
				break;
			}

			//To samo jest wklejone 2 razy, bo jak ca�y czas je�dzimy myszk� to kolejka event�w blokuje od�wierzanie planszy

			if (menu == BOARD && czyGraDziala) { //wykonaj obliczenia planszy je�li gra jest w��czona
				if (zegar.getElapsedTime().asMilliseconds() >= odstepCzasu) { //je�li min�� odpowiedni czas
					zegar.restart();
					cellMap.calculateMapa();
					if (board.pasteInEnabled) board.hologram(false);
					board.updateKwadraty();


					window.clear(background);	//rysowanie rzeczy zale�nie gdzie jeste�my
					window.draw(board);
					window.draw(gameMenu);
					window.display();
				}
			}
		}		

		if (menu == BOARD && czyGraDziala) { //wykonaj obliczenia planszy je�li gra jest w��czona
			if (zegar.getElapsedTime().asMilliseconds() >= odstepCzasu) { //je�li min�� odpowiedni czas
				zegar.restart();
				cellMap.calculateMapa();
				if (board.pasteInEnabled) board.hologram(false);
				board.updateKwadraty();
			}
		}

		window.clear(background);	//rysowanie rzeczy zale�nie gdzie jeste�my
		switch (menu) {
		case BOARD:
			window.draw(board);
			window.draw(gameMenu);
			break;
		case MAIN:
			window.draw(mainMenu);
		}
		
		window.display();
	}
	return 0;
}