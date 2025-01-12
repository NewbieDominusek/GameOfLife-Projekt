#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"

#define ILE_GUZIKOW 9 //ilo�� guzik�w w UI gry

class GameMenu : public sf::Drawable
{
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const; //potrzebne aby mo�na by�o bezpo�rednio przekaza� obiekt do window.draw()
	int oknoW, oknoH;	//rozmiar okna
	int origW, origH;	//oryginalny rozmiar okna
	int sliderVal = 0; //warto�� slidera (wyznacza pr�dko�� gry)
	Button scena[ILE_GUZIKOW];	//guziki

public:
	
	int getSliderVal(); //przeka� warto�� slidera
	GameMenu(int w, int h);	//konstruktor
	int handleEvent(sf::Event event); //handlowanie event�w
	void updateBttnText(int index, std::string tekst); //zaktualizuj tekst na guziku
};

