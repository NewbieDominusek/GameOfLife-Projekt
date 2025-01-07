#include "GameMenu.h"


void GameMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (int i = 0; i < ileGuzikow; i++)
	{
		target.draw(scena[i], states);	//rysowanie wszystkich guzików po kolei
	}

}

int GameMenu::handleEvent(sf::Event event) {	//handlowanie eventów
	int x, y;
	switch (event.type) {
	case sf::Event::MouseButtonPressed:
		x = event.mouseButton.x * origW / oknoW;	//skalujemy przekazan¹ pozycjê myszki do oryginalnych rozmiarów okna
		y = event.mouseButton.y * origH / oknoH;
		
		for (int i = 0; i < ileGuzikow; i++) {
			if (scena[i].mouseOnButton(x, y) == 1) { //pytamy siê guzików, czy to na nie klikniêto XD
				if (i == 1 || i == 2) {
					if (x < 270) x = 270;
					else if (x > 620) x = 620;
					scena[2].moveBttn(x - 7, 2);
					sliderVal = x - 270; //zakres 0 - 350
				}
				return i;	
			}
		}
		
		return -1;	//jak nie to dajemy -1 i elo
		break;

	case sf::Event::Resized:	//aktualizujemy zmienne z aktualnym rozmiarem okna
		oknoW = event.size.width;
		oknoH = event.size.height;
		return 0;

	case sf::Event::MouseWheelScrolled:
		x = event.mouseWheelScroll.x * origW / oknoW;	//skalujemy przekazan¹ pozycjê myszki do oryginalnych rozmiarów okna
		y = event.mouseWheelScroll.y * origH / oknoH;

		for (int i = 0; i < ileGuzikow; i++) {
			if (scena[i].mouseOnButton(x, y) == 1) return i; //pytamy siê guzików, czy to na nich scrollowano XD
		}

	default:
		return 0;
	}
}

GameMenu::GameMenu(int w, int h) {
	int charSize = 20;
	oknoW = w;
	oknoH = h;
	origW = w;
	origH = h;

	scena[0].setButton(100 ,20, 20, 5, "Main Menu", sf::Color::Red, charSize);	//pierwszy guzik
	scena[1].setButton(350, 20, 270, 5, " ", sf::Color::Red, charSize);	//slider body
	scena[2].setButton(15, 25, 300, 2, " ", sf::Color::Black, charSize);	//slider
	scena[3].setButton(50,20 ,20 ,420 , "PLAY", sf::Color::Green, charSize);	//PLAY
	scena[4].setButton(50, 20, 195, 420, "STOP", sf::Color::Red, charSize);	//STOP
	scena[5].setButton(250, 20, 370, 420, "PASTE IN", sf::Color::Yellow, charSize);	//PASTE IN
	scena[6].setButton(150, 20, 90, 445, "40", sf::Color::White, charSize);	//W
	scena[7].setButton(150, 20, 400, 445, "40", sf::Color::Green, charSize); //H
	scena[8].setButton(100, 20, 140, 5, "Reset", sf::Color::Red, charSize);	//RESET
}

int GameMenu::getSliderVal() {
	return sliderVal; //pozycja slidera mo¿e byæ min 263, odejmuj¹c 260 minimalna wartoœæ to 3. Wyznacza czas szybkoœæ gry
}

void GameMenu::updateBttnText(int index, std::string tekst) {
	if (index > ILE_GUZIKOW - 1) return;
	scena[index].changeText(tekst);
}