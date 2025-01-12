#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"

#define ILE_GUZIKOW 9 //iloœæ guzików w UI gry

class GameMenu : public sf::Drawable
{
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const; //potrzebne aby mo¿na by³o bezpoœrednio przekazaæ obiekt do window.draw()
	int oknoW, oknoH;	//rozmiar okna
	int origW, origH;	//oryginalny rozmiar okna
	int sliderVal = 0; //wartoœæ slidera (wyznacza prêdkoœæ gry)
	Button scena[ILE_GUZIKOW];	//guziki

public:
	
	int getSliderVal(); //przeka¿ wartoœæ slidera
	GameMenu(int w, int h);	//konstruktor
	int handleEvent(sf::Event event); //handlowanie eventów
	void updateBttnText(int index, std::string tekst); //zaktualizuj tekst na guziku
};

