#include "GameInstance.h"

GameInstance::GameInstance()
{
	window = new sf::RenderWindow(sf::VideoMode(windowWidth, windowHeight), "Game Of Life");
	//window->setTitle("Game Of Life");
	//window->setSize(sf::Vector2u(windowWidth, windowHeight));
	window->setFramerateLimit(120);
	cellMap = new Mapa(64, 40);
	mainMenu = new MainMenu(windowWidth, windowHeight);
	gameMenu = new GameMenu(windowWidth, windowHeight, cellMap->width, cellMap->height);
	board = new TileMap(cellMap, windowWidth, windowHeight);

	board->holoData = wzorki.patterns[wzorki.wybranyWzorek].data;	//pocz¹tkowy wzorek (domyœlny glider)
	board->holoWidth = wzorki.patterns[wzorki.wybranyWzorek].width;
	board->holoHeight = wzorki.patterns[wzorki.wybranyWzorek].height;
	gameMenu->updateBttnText(5, wzorki.patterns[wzorki.wybranyWzorek].name);

	board->drawBounds = TileMap::YES;	//czy rysowaæ krawêdzie miêdzy komórkami
	board->kolorLinii = sf::Color(100, 200, 40, 70);	//kolor krawêdzi
	board->updateLinie();	//narysuj linie
	board->updateKwadraty();		//metoda aktualizuje wygl¹d renderowanych komórek, nale¿y u¿yæ po ka¿dej zmianie tablicy map


}

GameInstance::~GameInstance() {
	delete board;
	delete gameMenu;
	delete mainMenu;
	delete cellMap;
	delete window;
}

void GameInstance::handleEvents()
{
	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event)) {	//handlowanie eventów
			switch (event.type) {	//zamkniêcie okna
			case sf::Event::Closed:
				window->close();	//zamknij okno
				break;

			case sf::Event::MouseButtonPressed:	//naciœniêcie myszki
				if (menu == MAIN) {	//jeœli jesteœmy w menu g³ównym
					switch (mainMenu->handleEvent(event)) {	//main sprawdza na który przycisk naciœniêto i zwraca jego index
					case 0:	//START
						menu = BOARD;	//zacznij grê
						break;
					case 1:
						ShellExecuteA(NULL, "open", ".", NULL, NULL, SW_SHOWDEFAULT);
						break;

					case 2:	//EXIT
						window->close();
					default:	//na nic nie naciœniêto (skill issue)
						break;
					}
				}
				else {
					switch (gameMenu->handleEvent(event)) {	//sprawdzamy klikniêty guzik
					case 0:	//MAIN MENU
						menu = MAIN;	//idŸ do menu
						czyGraDziala = false; //wy³¹cz grê
						break;
					case 1: //Slider body
						odstepCzasu = gameMenu->getSliderVal() * 3; //we¿ wartoœæ slidera (szybkoœæ gry)
						if (odstepCzasu < MIN_PLAY_TIME) odstepCzasu = MIN_PLAY_TIME; //nie mo¿e byæ za ma³a bo komputer jebnie xd
						break;
					case 2: //slider
						odstepCzasu = gameMenu->getSliderVal() * 3;
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
						board->pasteInEnabled = !board->pasteInEnabled; //prze³¹cz wklejanie hologramu
						board->showHolo = false; //wy³¹cz rysowanie hologramu (jesteœmy poza plansz¹)
						board->hologram(false); //wywo³anie funkcji aby ustawi³a planszê w odpowiedni stan
						board->updateKwadraty(); //odœwierzenie planszy
						break;


					case 6: //width (zwiêkszanie i zmniejszanie)
						if (event.mouseButton.button == sf::Mouse::Left) cellMap->resizePlansza(1, 0); //zwiêksz planszê
						else if (event.mouseButton.button == sf::Mouse::Right) cellMap->resizePlansza(-1, 0);
						gameMenu->updateBttnText(6, std::to_string(cellMap->width));
						board->resizePlansza();
						break;


					case 7: //height (zwiêkszanie i zmniejszanie)
						if (event.mouseButton.button == sf::Mouse::Left) cellMap->resizePlansza(0, 1);
						else if (event.mouseButton.button == sf::Mouse::Right) cellMap->resizePlansza(0, -1);
						gameMenu->updateBttnText(7, std::to_string(cellMap->height));
						board->resizePlansza();
						break;


					case 8: //RESET
						czyGraDziala = false; //wy³¹cz grê
						cellMap->resetMapa();
						board->updateKwadraty();
						break;

					case 9: //RESET
						cellMap->zawijanie = !cellMap->zawijanie;
						gameMenu->updateBttnColor(9, (cellMap->zawijanie) ? sf::Color::Green : sf::Color::Red);
						break;


					default:	//nie naciœniêto na guzik, wiêc mo¿e na planszê z komórkami
						board->HandleEvent(event);	//Jeœli jesteœmy w grze i nie naciœniêto guzika, to dajemy event do planszy (ona sobie poradzi)
						break;
					}

				}

				break;

			case sf::Event::Resized:	//zmieniono rozmiar ekranu, trzeba zaktualizowaæ zmienne z aktualnym rozmiarem ekranu (obiekty sobie poradz¹ ez pz)
				board->HandleEvent(event);
				mainMenu->handleEvent(event);
				gameMenu->handleEvent(event);
				break;

			case sf::Event::MouseWheelScrolled: //przescrollowano kó³kiem (rozmiar planszy mo¿na zmieniaæ kó³kiem)
				if (menu == BOARD) { //musimy byæ na ekranie z gr¹
					switch (gameMenu->handleEvent(event)) {	//sprawdzamy na co scrollowano

					case 5:
						wzorki.wybranyWzorek += event.mouseWheelScroll.delta; //dodajemy scrolla
						if (wzorki.wybranyWzorek < 0) wzorki.wybranyWzorek = wzorki.patterns.size() - 1; //jeœli przechodzimy do ty³u to przechodzimy na pocz¹tek
						wzorki.wybranyWzorek = wzorki.wybranyWzorek % wzorki.patterns.size(); //zawijamy jeœli liczba wiêksza ni¿ iloœæ elementów



						board->holoData = wzorki.patterns[wzorki.wybranyWzorek].data;	//pocz¹tkowy wzorek (domyœlny glider)
						board->holoWidth = wzorki.patterns[wzorki.wybranyWzorek].width;
						board->holoHeight = wzorki.patterns[wzorki.wybranyWzorek].height;
						gameMenu->updateBttnText(5, wzorki.patterns[wzorki.wybranyWzorek].name);
						break;

					case 6: //width (zwiêkszanie i zmniejszanie)
						cellMap->resizePlansza((int)event.mouseWheelScroll.delta, 0);

						gameMenu->updateBttnText(6, std::to_string(cellMap->width));
						board->resizePlansza();
						break;


					case 7: //height (zwiêkszanie i zmniejszanie)
						cellMap->resizePlansza(0, (int)event.mouseWheelScroll.delta);

						gameMenu->updateBttnText(7, std::to_string(cellMap->height));
						board->resizePlansza();
						break;
					}
				}
				break;

			case sf::Event::MouseMoved:
				board->HandleEvent(event);
				break;

			default:
				break;
			}

			//To samo jest wklejone 2 razy, bo jak ca³y czas jeŸdzimy myszk¹ to kolejka eventów blokuje odœwierzanie planszy

			if (menu == BOARD && czyGraDziala) { //wykonaj obliczenia planszy jeœli gra jest w³¹czona
				if (zegar.getElapsedTime().asMilliseconds() >= odstepCzasu) { //jeœli min¹³ odpowiedni czas
					zegar.restart();
					cellMap->calculateMapa();
					if (board->pasteInEnabled) board->hologram(false);
					board->updateKwadraty();


					window->clear(background);	//rysowanie rzeczy zale¿nie gdzie jesteœmy
					window->draw(*board);
					window->draw(*gameMenu);
					window->display();
				}
			}
		}

		if (menu == BOARD && czyGraDziala) { //wykonaj obliczenia planszy jeœli gra jest w³¹czona
			if (zegar.getElapsedTime().asMilliseconds() >= odstepCzasu) { //jeœli min¹³ odpowiedni czas
				zegar.restart();
				cellMap->calculateMapa();
				if (board->pasteInEnabled) board->hologram(false);
				board->updateKwadraty();
			}
		}

		window->clear(background);	//rysowanie rzeczy zale¿nie gdzie jesteœmy
		switch (menu) {
		case BOARD:
			window->draw(*board);
			window->draw(*gameMenu);
			break;
		case MAIN:
			window->draw(*mainMenu);
		}

		window->display();
	}
}
