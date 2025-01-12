#include <SFML/Graphics.hpp>
#include "kwadraty.h"
#include "MainMenu.h"
#include "GameMenu.h"
#include "PatternContainer.h"

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <windows.h>
#include <shellapi.h>

#define MIN_PLAY_TIME 20 //minimalny odst�p czasu mi�dzy obliczaniem kolejnej planszy

void resetMapa(); // wyzeruj map�
void resizePlansza(int oldW, int oldH, int w, int h);	//zmie� rozmiar mapy
void calculateMapa(int width, int height, int* map);	//oblicz nast�pn� tur�
int countNeighbours(int row, int col, int width, int height, int* map);		//policz �ywych s�siad�w kom�rki
int decideCellState(int neighbourCnt, int currentState);		//wylicz nast�pny stan kom�rki
void calculateNeighbours(int row, int col, int width, int height, int* map);	//oblicz s�siednie kom�rki

int height = 40, width = 40; //rozmiar szachownicy
int* map = (int*)calloc(height * width, sizeof(int)); //tablica z kom�rkami
int* temp; //pomocnicza do zmiany rozmiaru mapy

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

	MainMenu mainMenu(windowWidth, windowHeight);	//klasa menu g��wnego
	GameMenu gameMenu(windowWidth, windowHeight);	//klasa z kontrolkami do gry
	
	TileMap board(map, width, height, windowWidth, windowHeight); //klasa u�ywana do renderowania kom�rek, renderowanie jest bazowane na tablicy map, nale�y przekaza� tablic�, rozmiar tablicy i rozmiar kom�rek do wyrenderowania

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
				free(map);	//zwolnij tablic� z kom�rkami
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
						free(map);	//zamknij program
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
						if (event.mouseButton.button == sf::Mouse::Left) resizePlansza(height, width, height, width + 1); //zwi�ksz plansz�
						else if (event.mouseButton.button == sf::Mouse::Right) {
							if (width <= 3) break;
							resizePlansza(height, width, height, width - 1);
						}
						gameMenu.updateBttnText(6,std::to_string(width));
						board.resizePlansza(width, height, map);
						break;


					case 7: //height (zwi�kszanie i zmniejszanie)
						if (event.mouseButton.button == sf::Mouse::Left) resizePlansza(height, width, height + 1, width);
						else if (event.mouseButton.button == sf::Mouse::Right) {
							if (height <= 3) break;
							resizePlansza(height, width, height - 1, width);
						}
						gameMenu.updateBttnText(7,std::to_string(height));
						board.resizePlansza(width, height, map);
						break;


					case 8: //RESET
						czyGraDziala = false; //wy��cz gr�
						resetMapa();
						board.updateKwadraty();
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
						if (width <= 3 && event.mouseWheelScroll.delta < 0) break;
						if (width + event.mouseWheelScroll.delta <= 3) resizePlansza(height, width, height, 3);
						else resizePlansza(height, width, height, width + event.mouseWheelScroll.delta);

						gameMenu.updateBttnText(6, std::to_string(width));
						board.resizePlansza(width, height, map);
						break;


					case 7: //height (zwi�kszanie i zmniejszanie)
						if (height <= 3 && event.mouseWheelScroll.delta < 0) break;
						if (height + event.mouseWheelScroll.delta <= 3) resizePlansza(height, width, 3, width);
						else resizePlansza(height, width, height + event.mouseWheelScroll.delta, width);

						gameMenu.updateBttnText(7, std::to_string(height));
						board.resizePlansza(width, height, map);
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
					calculateMapa(width, height, map);
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
				calculateMapa(width, height, map);
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








void resetMapa() { //wyzeruj ca�� map�
	for (int i = 0; i < width * height; i++) {
		map[i] = 0; //przejd� po ca�ej mapie i wyzeruj
	}
}

void resizePlansza(int oldH, int oldW, int h, int w) { //zmiana rozmiaru g��wnej tablicy mapy
	temp = (int*)calloc(w * h, sizeof(int)); //tymczasowa zmienna do przechowania oryginalnych warto�ci kom�rek (rozmiar docelowej tablicy)
	int minW = (oldW >= w) ? w : oldW; //ustal kt�re wymiary s� mniejsze aby nie wyj�� poza rozmiar tablic
	int minH = (oldH >= h) ? h : oldH; //ustal kt�re wymiary s� mniejsze aby nie wyj�� poza rozmiar tablic
	for (int x = 0; x < minW; x++) { //[rzechodzimy po kom�rkach
		for (int y = 0; y < minH; y++) {
			temp[y * w + x] = map[y * oldW + x]; //przekopiuj kom�rki tablicy g��wnej do tymczasowej, je�li tymczasowa jest mniejsza to dane z g��wnej zostan� uci�te
		}
	}
	free(map); //usu� g��wn� tablic�
	map = (int*)calloc(w * h, sizeof(int)); //i stw�rz od nowa o nowych rozmiarach
	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			map[y * w + x] = temp[y * w + x]; //wklej dane z tablicy tymczasowej do g��wnej
		}
	}
	free(temp); //zwolnij tymczasow�
	height = h; //ustaw nowy rozmiar
	width = w;
}





//parzyste liczby to martwa kom�rka, nieparzyste to �ywa
//0 - martwa
//1 - �ywa
//2 - nowa martwa
//3 - nowa �ywa
// 
// hologramy:
//4 - martwa rysowana jako �ywa
//5 - �ywa rysowana jako martwa
//6 - martwa rysowana innym kolorem
//7 - �ywa rysowana innym kolorem

// <2 s�siad�w umiera
//2-3 s�siad�w prze�ywa
// >3 s�siad�w umiera
//  3 s�siad�w o�ywa


void calculateMapa(int width, int height, int *map) { //g��wna funkcja gry obliczaj�ca nast�pn� iteracj� planszy
	int liveNeighbourCount = 0; //�ywi s�siedzi
	int num = 0;	//numer kom�rki
	for (int row = 0; row < height; row++) { //przechodzimy po ca�ej mapie
		for (int col = 0; col < width; col++) { 

			num = row * width + col; //oblicz numer kom�rki z rz�du i kolumny
			if (map[num] > 3) map[num] = map[num] % 2; //usuwamy kom�rki pomalowane przez funkcj� hologramow�
			if (map[num] != 1) continue; //obliczamy tylko dla �ywych kom�rek, kt�re nie by�y jeszcze liczone (optymalizacja)
			map[num] = decideCellState(countNeighbours(row, col, width, height, map), map[num]); //liczymy s�siad�w obecnej kom�rki
			calculateNeighbours(row, col, width, height, map); //teraz obliczamy s�siad�w obecnej kom�rki (optymalizacja, obliczamy tylko �ywe kom�rki i ich s�siad�w, bo martwa kom�rka z martwymi s�siadami nie mo�e zmieni� stanu)
		}
	}

	for (int i = 0; i < height * width; i++) map[i] = map[i] % 2; //usu� robocze warto�ci kom�rek (obliczona tablica ma zawierac tylko 1 lub 0)
}

int decideCellState(int neighbourCnt, int currentState) { //funkcja decyduj�ca nast�pny stan kom�rki na podstawie liczby s�siad�w
	if (neighbourCnt < 2) return (currentState == 1) ? 2 : 0;	//ma za ma�o s�siad�w i jest �ywa, to ma um�e�
	else if (neighbourCnt > 3) return (currentState == 1) ? 2 : 0;	//ma za du�o s�siad�w i jest �ywa to ma um�e�
	else if (neighbourCnt == 3) return (currentState == 0) ? 3 : currentState;	//ma 3 s�siad�w i jest martwa to ma o�y�
	else return currentState; //stan ma pozosta� niezmieniony
}

void calculateNeighbours(int row, int col, int width, int height, int* map) { //oblicz s�siad�w podanej kom�rki
	int above = (row == 0) ? height - 1 : row - 1, below = (row == height - 1) ? 0 : row + 1; //rz�dy na g�rze i dole, je�li wychodzi poza map� to zawijamy
	int left = (col == 0) ? width - 1 : col - 1, right = (col == width - 1) ? 0 : col + 1; // to samo dla kolumn

	int coords[16] = {	//koordynaty kom�rek do sprawdzenia
		above, left,	//te kom�rki s� wok� obecnie sprawdzanej kom�rki g��wnej (�rodkowej)
		above, col,
		above, right,
		row, left,
		row, right,
		below, left,
		below, col,
		below, right
	};

	for (int i = 0; i < 16; i += 2) { //przejd� po kom�rkach do sprawdzenia
		int num = coords[i] * width + coords[i + 1]; //numer kom�rki
		if (map[num] > 3) map[num] = map[num] % 2; //usuwamy warto�ci robocze ustawione przez hologram
		if (map[num] != 0) continue; //obliczamy tylko martwe kom�rki (�ywe zostan� obliczone przez p�tl� g��wnej funkcji)
		map[num] = decideCellState(countNeighbours(coords[i], coords[i + 1], width, height, map), map[num]); //oblicz nowy stan kom�rki
	}
		
}

int countNeighbours(int row, int col, int width, int height, int *map) { //policz �ywych s�siad�w kom�rki
	int count = 0;
	int above = (row == 0) ? height - 1 : row - 1, below = (row == height - 1) ? 0 : row + 1; //rz�dy na g�rze i dole, je�li wychodzi poza map� to zawijamy
	int left = (col == 0) ? width - 1 : col - 1, right = (col == width - 1) ? 0 : col + 1; // to samo dla kolumn
	int numsToCheck[8] = { // numery kom�rek wok� naszej sprawdzanej
	above * width + left,
	above* width + col,
	above* width + right,
	row * width + left,
	row* width + right,
	below* width + left,
	below* width + col,
	below* width + right,
	};
	for (int i = 0; i < 8; i++) {
		if (map[numsToCheck[i]] > 3) map[numsToCheck[i]] = map[numsToCheck[i]] % 2;//usuwamy pomalowane kom�rki
		if (map[numsToCheck[i]] == 1 || map[numsToCheck[i]] == 2) count++; //�ywa albo nowa martwa (by�a �ywa)
	}
	return count;
}