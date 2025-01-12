#include "MainMenu.h"
#include <SFML/Graphics.hpp>

void MainMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (int i = 0; i < 3; i++)
	{
		target.draw(scena[i], states);	//rysowanie wszystkich guzik�w po kolei
	}
	
}

MainMenu::MainMenu(int w, int h) {
	oknoW = w;
	oknoH = h;
	origW = w;
	origH = h;
	int margin = 10;	//margines mi�dzy guzikami
	int bttnW = origW/4;	//szeroko�� guzik�w
	int bttnH = origH / ILE_GUZIKOW;	//wysoko�� (dzielimy wysoko�c okna r�wno mi�dzy guziki)
	scena[0].setButton(bttnW - margin, bttnH - margin, margin / 2 + origW / 2 - bttnW / 2, bttnH * 0 + margin/2, "Start", sf::Color::Green,60);	//pierwszy guzik START
	scena[1].setButton(bttnW - margin, bttnH - margin, margin / 2 + origW / 2 - bttnW / 2, bttnH * 1 + margin / 2, "Edit", sf::Color::Yellow,60);	//guzik EDIT (otwiera lokalizacj� gry)
	scena[2].setButton(bttnW - margin, bttnH - margin, margin / 2 + origW / 2 - bttnW / 2, bttnH * 2 + margin / 2, "Exit", sf::Color::Red,60);		//EXIT		Nale�y pami�ta� o marginesach
}

int MainMenu::handleEvent(sf::Event event) {	//handlowanie event�w
	int x, y;
	switch (event.type) {
	case sf::Event::MouseButtonPressed:
			x = event.mouseButton.x * origW / oknoW;	//skalujemy przekazan� pozycj� myszki do oryginalnych rozmiar� okna
			y = event.mouseButton.y * origH / oknoH;
			for (int i = 0; i < ILE_GUZIKOW; i++) {
				if (scena[i].mouseOnButton(x, y) == 1) return i;	//pytamy si� guzik�w, czy to na nie klikni�to
			}
			return -1;	//jak nie to dajemy -1
		break;

	case sf::Event::Resized:	//aktualizujemy zmienne z aktualnym rozmiarem okna
		oknoW = event.size.width;
		oknoH = event.size.height;
		return 0;
		
	default:
		return 0;
	}
}