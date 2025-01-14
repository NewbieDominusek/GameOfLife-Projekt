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

int main()
{
	//DEFINICJE WZORÓW DO WKLEJENIA Z PLIKU "patterns.txt" PLIK MUSI BYÆ W FOLDERZE Z .exe
	//format pliku:
	//h:[wysokoœæ].
	//w:[szerokoœæ].
	//d:
	//001
	//101
	//011.
	//p:[Nazwa wzorku].

	//zapisujemy dok³adnie z kropkami na koñcu segmentu danych i dwukropkami.
	//nazwa wzorku, czyli p (pattern) musi byæ poprzedzone danymi h d w (nie 3 nie musz¹ byæ w dok³adniej kolejnoœci)
	//ten format mo¿na powtarzaæ dowoln¹ iloœæ razy aby zapisaæ wiele wzorków
	//podane rozmiary wzorku musz¹ siê zgadzaæ z iloœci¹ danych, inaczej wzorek bêdzie zignorowany
	PatternContainer wzorki;
	int wybranyWzorek = 0;

	int windowWidth = 640, windowHeight = 480; //pocz¹tkowy rozmiar okna
	sf::RenderWindow window(	//Zrób okno
		sf::VideoMode(windowWidth, windowHeight),
		"Game Of Life");
	window.setFramerateLimit(120);

	enum MENU {		//na którym ekranie jesteœmy
		BOARD,	//gra
		MAIN	//menu g³ówne
	};

	MENU menu = MAIN;	//zmienna do wyboru ekranu (startujemy na g³ównym menu)

	const sf::Color background = sf::Color(100, 100, 100, 150); //kolor t³a

	Mapa cellMap(3,3);
	MainMenu mainMenu(windowWidth, windowHeight);	//klasa menu g³ównego
	GameMenu gameMenu(windowWidth, windowHeight, cellMap.width, cellMap.height);	//klasa z kontrolkami do gry
	
	TileMap board(&cellMap, windowWidth, windowHeight); //klasa u¿ywana do renderowania komórek, renderowanie jest bazowane na tablicy map, nale¿y przekazaæ tablicê, rozmiar tablicy i rozmiar komórek do wyrenderowania

	board.holoData = wzorki.patterns[wybranyWzorek].data;	//pocz¹tkowy wzorek (domyœlny glider)
	board.holoWidth = wzorki.patterns[wybranyWzorek].width;
	board.holoHeight = wzorki.patterns[wybranyWzorek].height;
	gameMenu.updateBttnText(5,wzorki.patterns[wybranyWzorek].name);

	board.drawBounds = TileMap::YES;	//czy rysowaæ krawêdzie miêdzy komórkami
	board.kolorLinii = sf::Color(100, 200, 40, 70);	//kolor krawêdzi
	board.updateLinie();	//narysuj linie
	board.updateKwadraty();		//metoda aktualizuje wygl¹d renderowanych komórek, nale¿y u¿yæ po ka¿dej zmianie tablicy map

	sf::Clock zegar;	//obiekt z zegarem (do gry)
	bool czyGraDziala = false;	//czy gra jest w³¹czona (obliczanie komórek)
	int odstepCzasu = 200;		//odstêp miêdzy obliczeniami w milisekundach (default 200)


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event)) {	//handlowanie eventów
			switch (event.type) {	//zamkniêcie okna
			case sf::Event::Closed:
				window.close();	//zamknij okno
				break;

			case sf::Event::MouseButtonPressed:	//naciœniêcie myszki
				if (menu == MAIN) {	//jeœli jesteœmy w menu g³ównym
					switch (mainMenu.handleEvent(event)) {	//main sprawdza na który przycisk naciœniêto i zwraca jego index
					case 0:	//START
						menu = BOARD;	//zacznij grê
						break;
					case 1:
						ShellExecuteA(NULL, "open", ".", NULL, NULL, SW_SHOWDEFAULT);
						break;

					case 2:	//EXIT
						window.close();
					default:	//na nic nie naciœniêto (skill issue)
						break;
					}
				}
				else {
					switch (gameMenu.handleEvent(event)) {	//sprawdzamy klikniêty guzik
					case 0:	//MAIN MENU
						menu = MAIN;	//idŸ do menu
						czyGraDziala = false; //wy³¹cz grê
						break;
					case 1: //Slider body
						odstepCzasu = gameMenu.getSliderVal() * 3; //we¿ wartoœæ slidera (szybkoœæ gry)
						if (odstepCzasu < MIN_PLAY_TIME) odstepCzasu = MIN_PLAY_TIME; //nie mo¿e byæ za ma³a bo komputer jebnie xd
						break;
					case 2: //slider
						odstepCzasu = gameMenu.getSliderVal() * 3;
						if (odstepCzasu < MIN_PLAY_TIME) odstepCzasu = MIN_PLAY_TIME;
						break;
					case 3: //PLAY
						czyGraDziala = true; //w³¹cz grê
						zegar.restart();	//wyzeruj timer
						break;
					case 4: //STOP
						czyGraDziala = false; //wy³¹cz grê
						break;

					case 5://PASTE IN (HOLOGRAM)
						board.pasteInEnabled = !board.pasteInEnabled; //prze³¹cz wklejanie hologramu
						board.showHolo = false; //wy³¹cz rysowanie hologramu (jesteœmy poza plansz¹)
						board.hologram(false); //wywo³anie funkcji aby ustawi³a planszê w odpowiedni stan
						board.updateKwadraty(); //odœwierzenie planszy
						break;


					case 6: //width (zwiêkszanie i zmniejszanie)
						if (event.mouseButton.button == sf::Mouse::Left) cellMap.resizePlansza(1,0); //zwiêksz planszê
						else if (event.mouseButton.button == sf::Mouse::Right) cellMap.resizePlansza(-1,0);
						gameMenu.updateBttnText(6,std::to_string(cellMap.width));
						board.resizePlansza();
						break;


					case 7: //height (zwiêkszanie i zmniejszanie)
						if (event.mouseButton.button == sf::Mouse::Left) cellMap.resizePlansza(0, 1);
						else if (event.mouseButton.button == sf::Mouse::Right) cellMap.resizePlansza(0, -1);
						gameMenu.updateBttnText(7,std::to_string(cellMap.height));
						board.resizePlansza();
						break;


					case 8: //RESET
						czyGraDziala = false; //wy³¹cz grê
						cellMap.resetMapa();
						board.updateKwadraty();
						break;

					case 9: //RESET
						cellMap.zawijanie = !cellMap.zawijanie;
						gameMenu.updateBttnColor(9, (cellMap.zawijanie) ? sf::Color::Green : sf::Color::Red);
						break;


					default:	//nie naciœniêto na guzik, wiêc mo¿e na planszê z komórkami
						board.HandleEvent(event);	//Jeœli jesteœmy w grze i nie naciœniêto guzika, to dajemy event do planszy (ona sobie poradzi)
						break;
					}
					
				}
				
				break;

			case sf::Event::Resized:	//zmieniono rozmiar ekranu, trzeba zaktualizowaæ zmienne z aktualnym rozmiarem ekranu (obiekty sobie poradz¹ ez pz)
				board.HandleEvent(event);
				mainMenu.handleEvent(event);
				gameMenu.handleEvent(event);
				break;

			case sf::Event::MouseWheelScrolled: //przescrollowano kó³kiem (rozmiar planszy mo¿na zmieniaæ kó³kiem)
				if (menu == BOARD) { //musimy byæ na ekranie z gr¹
					switch (gameMenu.handleEvent(event)) {	//sprawdzamy na co scrollowano

					case 5:
						wybranyWzorek += event.mouseWheelScroll.delta; //dodajemy scrolla
						if (wybranyWzorek < 0) wybranyWzorek = wzorki.patterns.size() -1; //jeœli przechodzimy do ty³u to przechodzimy na pocz¹tek
						wybranyWzorek = wybranyWzorek % wzorki.patterns.size(); //zawijamy jeœli liczba wiêksza ni¿ iloœæ elementów
						
						

						board.holoData = wzorki.patterns[wybranyWzorek].data;	//pocz¹tkowy wzorek (domyœlny glider)
						board.holoWidth = wzorki.patterns[wybranyWzorek].width;
						board.holoHeight = wzorki.patterns[wybranyWzorek].height;
						gameMenu.updateBttnText(5, wzorki.patterns[wybranyWzorek].name);
						break;

					case 6: //width (zwiêkszanie i zmniejszanie)
						cellMap.resizePlansza((int)event.mouseWheelScroll.delta, 0);

						gameMenu.updateBttnText(6, std::to_string(cellMap.width));
						board.resizePlansza();
						break;


					case 7: //height (zwiêkszanie i zmniejszanie)
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

			//To samo jest wklejone 2 razy, bo jak ca³y czas jeŸdzimy myszk¹ to kolejka eventów blokuje odœwierzanie planszy

			if (menu == BOARD && czyGraDziala) { //wykonaj obliczenia planszy jeœli gra jest w³¹czona
				if (zegar.getElapsedTime().asMilliseconds() >= odstepCzasu) { //jeœli min¹³ odpowiedni czas
					zegar.restart();
					cellMap.calculateMapa();
					if (board.pasteInEnabled) board.hologram(false);
					board.updateKwadraty();


					window.clear(background);	//rysowanie rzeczy zale¿nie gdzie jesteœmy
					window.draw(board);
					window.draw(gameMenu);
					window.display();
				}
			}
		}		

		if (menu == BOARD && czyGraDziala) { //wykonaj obliczenia planszy jeœli gra jest w³¹czona
			if (zegar.getElapsedTime().asMilliseconds() >= odstepCzasu) { //jeœli min¹³ odpowiedni czas
				zegar.restart();
				cellMap.calculateMapa();
				if (board.pasteInEnabled) board.hologram(false);
				board.updateKwadraty();
			}
		}

		window.clear(background);	//rysowanie rzeczy zale¿nie gdzie jesteœmy
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