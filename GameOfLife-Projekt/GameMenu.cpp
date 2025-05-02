#include "GameMenu.h"


void GameMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (int i = 0; i < ILE_GUZIKOW; i++)
	{
		target.draw(scena[i], states);	//rysowanie wszystkich guzików po kolei
	}

}

int GameMenu::handleEvent(sf::Event event) {	//handlowanie eventów
	int x, y;
	switch (event.type) {
	case sf::Event::MouseButtonPressed: //naciœniêcie przycisku myszki
		x = event.mouseButton.x * origW / oknoW;	//skalujemy przekazan¹ pozycjê myszki do oryginalnych rozmiarów okna
		y = event.mouseButton.y * origH / oknoH;
		
		for (int i = 0; i < ILE_GUZIKOW; i++) { //sprawdzamy który guzik naciœniêto
			if (scena[i].mouseOnButton(x, y) == 1) {
				if (i == 1 || i == 2) { //jeœli jest to slider to robimy inn¹ logikê
					if (x < 270) x = 270; //ograniczamy wysuniêcie slidera
					else if (x > 620) x = 620;
					scena[2].moveBttn(x - 7, 2); //przesuwamy slider na miejsce klikniêcia
					sliderVal = x - 270; //wartoœæ slidera (zakres 0 - 350)
				}
				return i; //zwracamy indeks guzika
			}
		}
		
		return -1;	//jak nie naciœniêto na guzik to zwracamy -1
		break;

	case sf::Event::Resized: //event zmiany rozmiaru okna
		oknoW = event.size.width; //aktualizujemy zmienne z aktualnym rozmiarem okna
		oknoH = event.size.height; //(potrzebne do skalowania pozycji myszki)
		return 0;

	case sf::Event::MouseWheelScrolled: //event scrollowania
		x = event.mouseWheelScroll.x * origW / oknoW;	//skalujemy przekazan¹ pozycjê myszki do oryginalnych rozmiarów okna
		y = event.mouseWheelScroll.y * origH / oknoH;

		for (int i = 0; i < ILE_GUZIKOW; i++) {
			if (scena[i].mouseOnButton(x, y) == 1) return i; //sprawdzamy na którym guziku scrollowano
		}

	default:
		return 0;
	}
}

GameMenu::GameMenu(int w, int h, int boardWidth, int boardHeight) { //stworzenie menu gry
	int charSize = 18; //rozmiar czcionki
	oknoW = w; //wartoœci do odpowiedniego skalowania
	oknoH = h;
	origW = w;
	origH = h;

	//stworzenie guzików
	scena[0].setButton(100 ,20, 20, 5, "Main Menu", sf::Color::Red, charSize);	//pierwszy guzik
	scena[1].setButton(350, 20, 270, 5, "<< Faster  -- Simulation Speed --  Slower >>", sf::Color::Red, charSize);	//slider body
	scena[2].setButton(15, 25, 300, 2, " ", sf::Color::Black, charSize);	//slider
	scena[3].setButton(50,20 ,20 ,420 , "PLAY", sf::Color::Green, charSize);	//PLAY
	scena[4].setButton(50, 20, 100, 420, "STOP", sf::Color::Red, charSize);	//STOP
	scena[5].setButton(250, 20, 370, 420, "PASTE IN", sf::Color::Yellow, charSize);	//PASTE IN
	scena[6].setButton(150, 20, 90, 445, std::to_string(boardWidth), sf::Color::White, charSize);	//W
	scena[7].setButton(150, 20, 400, 445, std::to_string(boardHeight), sf::Color::Cyan, charSize); //H
	scena[8].setButton(100, 20, 140, 5, "Reset", sf::Color::Red, charSize);	//RESET
	scena[9].setButton(100, 20, 200, 420, "Zawijanie", sf::Color::Green, charSize);	//ZAWIJANIE
}

int GameMenu::getSliderVal() { //getter wartoœci slidera
	return sliderVal;
}

void GameMenu::updateBttnText(int index, std::string tekst) { //zmiana tekstu guzika
	if (index > ILE_GUZIKOW - 1 || index < 0) return;
	scena[index].changeText(tekst);
}

void GameMenu::updateBttnColor(int index, sf::Color nowyKolor) {
	if (index > ILE_GUZIKOW - 1 || index < 0) return;
	scena[index].changeColor(nowyKolor);
}