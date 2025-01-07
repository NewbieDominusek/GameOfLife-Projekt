#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"

#define ILE_GUZIKOW 3

class MainMenu : public sf::Drawable
{
	const int ileGuzikow = ILE_GUZIKOW;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const; //potrzebne aby mo�na by�o bezpo�rednio przekaza� obiekt do window.draw()
	int oknoW, oknoH;	//rozmiar okna
	int origW, origH;	//oryginalny rozmiar okna
	Button scena[ILE_GUZIKOW];	//guziki :3


public:
	MainMenu(int w, int h);	//konstruktor
	int handleEvent(sf::Event event); //handlowanie event�w
};

