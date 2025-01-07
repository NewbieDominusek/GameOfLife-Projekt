#include "MainMenu.h"
#include <SFML/Graphics.hpp>

void MainMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (int i = 0; i < 3; i++)
	{
		target.draw(scena[i], states);	//rysowanie wszystkich guzików po kolei
	}
	
}

MainMenu::MainMenu(int w, int h) {
	oknoW = w;
	oknoH = h;
	origW = w;
	origH = h;
	int margin = 10;	//margines miêdzy guzikami
	int bttnW = origW/4;	//szerokoœæ guzików
	int bttnH = origH / ileGuzikow;	//wysokoœæ (dzielimy wysokoœc okna równo miêdzy guziki)
	int i = 0;	//licznik guzików, trzeba pilnowaæ ¿eby ileGuzików = faktyczna liczba guzików bo inaczej kaplica
	scena[i].setButton(bttnW - margin, bttnH - margin, margin / 2 + origW / 2 - bttnW / 2, bttnH * i++ + margin/2, "Start", sf::Color::Green,60);	//pierwszy guzik
	scena[i].setButton(bttnW - margin, bttnH - margin, margin / 2 + origW / 2 - bttnW / 2, bttnH * i++ + margin / 2, "Edit", sf::Color::Yellow,60);	//itp
	scena[i].setButton(bttnW - margin, bttnH - margin, margin / 2 + origW / 2 - bttnW / 2, bttnH * i++ + margin / 2, "Exit", sf::Color::Red,60);		//itd		Nale¿y pamiêtaæ o marginesach
}

int MainMenu::handleEvent(sf::Event event) {	//handlowanie eventów
	int x, y;
	switch (event.type) {
	case sf::Event::MouseButtonPressed:
			x = event.mouseButton.x * origW / oknoW;	//skalujemy przekazan¹ pozycjê myszki do oryginalnych rozmiaró okna
			y = event.mouseButton.y * origH / oknoH;
			for (int i = 0; i < ileGuzikow; i++) {
				if (scena[i].mouseOnButton(x, y) == 1) return i;	//pytamy siê guzików, czy to na nie klikniêto XD
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