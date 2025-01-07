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
	int bttnH = origH / ileGuzikow;	//wysoko�� (dzielimy wysoko�c okna r�wno mi�dzy guziki)
	int i = 0;	//licznik guzik�w, trzeba pilnowa� �eby ileGuzik�w = faktyczna liczba guzik�w bo inaczej kaplica
	scena[i].setButton(bttnW - margin, bttnH - margin, margin / 2 + origW / 2 - bttnW / 2, bttnH * i++ + margin/2, "Start", sf::Color::Green,60);	//pierwszy guzik
	scena[i].setButton(bttnW - margin, bttnH - margin, margin / 2 + origW / 2 - bttnW / 2, bttnH * i++ + margin / 2, "Edit", sf::Color::Yellow,60);	//itp
	scena[i].setButton(bttnW - margin, bttnH - margin, margin / 2 + origW / 2 - bttnW / 2, bttnH * i++ + margin / 2, "Exit", sf::Color::Red,60);		//itd		Nale�y pami�ta� o marginesach
}

int MainMenu::handleEvent(sf::Event event) {	//handlowanie event�w
	int x, y;
	switch (event.type) {
	case sf::Event::MouseButtonPressed:
			x = event.mouseButton.x * origW / oknoW;	//skalujemy przekazan� pozycj� myszki do oryginalnych rozmiar� okna
			y = event.mouseButton.y * origH / oknoH;
			for (int i = 0; i < ileGuzikow; i++) {
				if (scena[i].mouseOnButton(x, y) == 1) return i;	//pytamy si� guzik�w, czy to na nie klikni�to XD
			}
			return -1;	//jak nie to dajemy -1 i elo
		break;

	case sf::Event::Resized:	//aktualizujemy zmienne z aktualnym rozmiarem okna
		oknoW = event.size.width;
		oknoH = event.size.height;
		return 0;
		
	default:
		return 0;
	}
}