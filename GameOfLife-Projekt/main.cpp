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

#define MIN_PLAY_TIME 20 //minimalny odstêp czasu miêdzy obliczaniem kolejnej planszy

void resetMapa(); // wyzeruj mapê
void resizePlansza(int oldW, int oldH, int w, int h);	//zmieñ rozmiar mapy
void calculateMapa(int width, int height, int* map);	//oblicz nastêpn¹ turê
int countNeighbours(int row, int col, int width, int height, int* map);		//policz ¿ywych s¹siadów komórki
int decideCellState(int neighbourCnt, int currentState);		//wylicz nastêpny stan komórki
void calculateNeighbours(int row, int col, int width, int height, int* map);	//oblicz s¹siednie komórki

int height = 40, width = 40; //rozmiar szachownicy
int* map = (int*)calloc(height * width, sizeof(int)); //tablica z komórkami
int* temp; //pomocnicza do zmiany rozmiaru mapy

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

	MainMenu mainMenu(windowWidth, windowHeight);	//klasa menu g³ównego
	GameMenu gameMenu(windowWidth, windowHeight);	//klasa z kontrolkami do gry
	
	TileMap board(map, width, height, windowWidth, windowHeight); //klasa u¿ywana do renderowania komórek, renderowanie jest bazowane na tablicy map, nale¿y przekazaæ tablicê, rozmiar tablicy i rozmiar komórek do wyrenderowania

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
				free(map);	//zwolnij tablicê z komórkami
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
						free(map);	//zamknij program
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
						if (event.mouseButton.button == sf::Mouse::Left) resizePlansza(height, width, height, width + 1); //zwiêksz planszê
						else if (event.mouseButton.button == sf::Mouse::Right) {
							if (width <= 3) break;
							resizePlansza(height, width, height, width - 1);
						}
						gameMenu.updateBttnText(6,std::to_string(width));
						board.resizePlansza(width, height, map);
						break;


					case 7: //height (zwiêkszanie i zmniejszanie)
						if (event.mouseButton.button == sf::Mouse::Left) resizePlansza(height, width, height + 1, width);
						else if (event.mouseButton.button == sf::Mouse::Right) {
							if (height <= 3) break;
							resizePlansza(height, width, height - 1, width);
						}
						gameMenu.updateBttnText(7,std::to_string(height));
						board.resizePlansza(width, height, map);
						break;


					case 8: //RESET
						czyGraDziala = false; //wy³¹cz grê
						resetMapa();
						board.updateKwadraty();
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
						if (width <= 3 && event.mouseWheelScroll.delta < 0) break;
						if (width + event.mouseWheelScroll.delta <= 3) resizePlansza(height, width, height, 3);
						else resizePlansza(height, width, height, width + event.mouseWheelScroll.delta);

						gameMenu.updateBttnText(6, std::to_string(width));
						board.resizePlansza(width, height, map);
						break;


					case 7: //height (zwiêkszanie i zmniejszanie)
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

			//To samo jest wklejone 2 razy, bo jak ca³y czas jeŸdzimy myszk¹ to kolejka eventów blokuje odœwierzanie planszy

			if (menu == BOARD && czyGraDziala) { //wykonaj obliczenia planszy jeœli gra jest w³¹czona
				if (zegar.getElapsedTime().asMilliseconds() >= odstepCzasu) { //jeœli min¹³ odpowiedni czas
					zegar.restart();
					calculateMapa(width, height, map);
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
				calculateMapa(width, height, map);
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








void resetMapa() { //wyzeruj ca³¹ mapê
	for (int i = 0; i < width * height; i++) {
		map[i] = 0; //przejdŸ po ca³ej mapie i wyzeruj
	}
}

void resizePlansza(int oldH, int oldW, int h, int w) { //zmiana rozmiaru g³ównej tablicy mapy
	temp = (int*)calloc(w * h, sizeof(int)); //tymczasowa zmienna do przechowania oryginalnych wartoœci komórek (rozmiar docelowej tablicy)
	int minW = (oldW >= w) ? w : oldW; //ustal które wymiary s¹ mniejsze aby nie wyjœæ poza rozmiar tablic
	int minH = (oldH >= h) ? h : oldH; //ustal które wymiary s¹ mniejsze aby nie wyjœæ poza rozmiar tablic
	for (int x = 0; x < minW; x++) { //[rzechodzimy po komórkach
		for (int y = 0; y < minH; y++) {
			temp[y * w + x] = map[y * oldW + x]; //przekopiuj komórki tablicy g³ównej do tymczasowej, jeœli tymczasowa jest mniejsza to dane z g³ównej zostan¹ uciête
		}
	}
	free(map); //usuñ g³ówn¹ tablicê
	map = (int*)calloc(w * h, sizeof(int)); //i stwórz od nowa o nowych rozmiarach
	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			map[y * w + x] = temp[y * w + x]; //wklej dane z tablicy tymczasowej do g³ównej
		}
	}
	free(temp); //zwolnij tymczasow¹
	height = h; //ustaw nowy rozmiar
	width = w;
}





//parzyste liczby to martwa komórka, nieparzyste to ¿ywa
//0 - martwa
//1 - ¿ywa
//2 - nowa martwa
//3 - nowa ¿ywa
// 
// hologramy:
//4 - martwa rysowana jako ¿ywa
//5 - ¿ywa rysowana jako martwa
//6 - martwa rysowana innym kolorem
//7 - ¿ywa rysowana innym kolorem

// <2 s¹siadów umiera
//2-3 s¹siadów prze¿ywa
// >3 s¹siadów umiera
//  3 s¹siadów o¿ywa


void calculateMapa(int width, int height, int *map) { //g³ówna funkcja gry obliczaj¹ca nastêpn¹ iteracjê planszy
	int liveNeighbourCount = 0; //¿ywi s¹siedzi
	int num = 0;	//numer komórki
	for (int row = 0; row < height; row++) { //przechodzimy po ca³ej mapie
		for (int col = 0; col < width; col++) { 

			num = row * width + col; //oblicz numer komórki z rzêdu i kolumny
			if (map[num] > 3) map[num] = map[num] % 2; //usuwamy komórki pomalowane przez funkcjê hologramow¹
			if (map[num] != 1) continue; //obliczamy tylko dla ¿ywych komórek, które nie by³y jeszcze liczone (optymalizacja)
			map[num] = decideCellState(countNeighbours(row, col, width, height, map), map[num]); //liczymy s¹siadów obecnej komórki
			calculateNeighbours(row, col, width, height, map); //teraz obliczamy s¹siadów obecnej komórki (optymalizacja, obliczamy tylko ¿ywe komórki i ich s¹siadów, bo martwa komórka z martwymi s¹siadami nie mo¿e zmieniæ stanu)
		}
	}

	for (int i = 0; i < height * width; i++) map[i] = map[i] % 2; //usuñ robocze wartoœci komórek (obliczona tablica ma zawierac tylko 1 lub 0)
}

int decideCellState(int neighbourCnt, int currentState) { //funkcja decyduj¹ca nastêpny stan komórki na podstawie liczby s¹siadów
	if (neighbourCnt < 2) return (currentState == 1) ? 2 : 0;	//ma za ma³o s¹siadów i jest ¿ywa, to ma um¿eæ
	else if (neighbourCnt > 3) return (currentState == 1) ? 2 : 0;	//ma za du¿o s¹siadów i jest ¿ywa to ma um¿eæ
	else if (neighbourCnt == 3) return (currentState == 0) ? 3 : currentState;	//ma 3 s¹siadów i jest martwa to ma o¿yæ
	else return currentState; //stan ma pozostaæ niezmieniony
}

void calculateNeighbours(int row, int col, int width, int height, int* map) { //oblicz s¹siadów podanej komórki
	int above = (row == 0) ? height - 1 : row - 1, below = (row == height - 1) ? 0 : row + 1; //rzêdy na górze i dole, jeœli wychodzi poza mapê to zawijamy
	int left = (col == 0) ? width - 1 : col - 1, right = (col == width - 1) ? 0 : col + 1; // to samo dla kolumn

	int coords[16] = {	//koordynaty komórek do sprawdzenia
		above, left,	//te komórki s¹ wokó³ obecnie sprawdzanej komórki g³ównej (œrodkowej)
		above, col,
		above, right,
		row, left,
		row, right,
		below, left,
		below, col,
		below, right
	};

	for (int i = 0; i < 16; i += 2) { //przejdŸ po komórkach do sprawdzenia
		int num = coords[i] * width + coords[i + 1]; //numer komórki
		if (map[num] > 3) map[num] = map[num] % 2; //usuwamy wartoœci robocze ustawione przez hologram
		if (map[num] != 0) continue; //obliczamy tylko martwe komórki (¿ywe zostan¹ obliczone przez pêtlê g³ównej funkcji)
		map[num] = decideCellState(countNeighbours(coords[i], coords[i + 1], width, height, map), map[num]); //oblicz nowy stan komórki
	}
		
}

int countNeighbours(int row, int col, int width, int height, int *map) { //policz ¿ywych s¹siadów komórki
	int count = 0;
	int above = (row == 0) ? height - 1 : row - 1, below = (row == height - 1) ? 0 : row + 1; //rzêdy na górze i dole, jeœli wychodzi poza mapê to zawijamy
	int left = (col == 0) ? width - 1 : col - 1, right = (col == width - 1) ? 0 : col + 1; // to samo dla kolumn
	int numsToCheck[8] = { // numery komórek wokó³ naszej sprawdzanej
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
		if (map[numsToCheck[i]] > 3) map[numsToCheck[i]] = map[numsToCheck[i]] % 2;//usuwamy pomalowane komórki
		if (map[numsToCheck[i]] == 1 || map[numsToCheck[i]] == 2) count++; //¿ywa albo nowa martwa (by³a ¿ywa)
	}
	return count;
}