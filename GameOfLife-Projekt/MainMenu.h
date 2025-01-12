#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"

#define ILE_GUZIKOW 3

class MainMenu : public sf::Drawable
{
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const; //potrzebne aby mo¿na by³o bezpoœrednio przekazaæ obiekt do window.draw()
	int oknoW, oknoH;	//rozmiar okna
	int origW, origH;	//oryginalny rozmiar okna
	Button scena[ILE_GUZIKOW];	//guziki


public:
	MainMenu(int w, int h);	//konstruktor
	int handleEvent(sf::Event event); //handlowanie eventów
};

