#pragma once
#include <SFML/Graphics.hpp>
#include <string.h>

class Button : public sf::Drawable
{
	sf::Font czcionka;	//czcionka guzika
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const; //potrzebne aby mo�na by�o bezpo�rednio przekaza� obiekt do window.draw()
	sf::Text text;	//tekst na guziku
	int height, width, posX, posY;	//rozmiary i pozycja guzika
	sf::RectangleShape rectangle;	//kwadrat (guzik)

	

public:
	int mouseOnButton(int x, int y);	//Sprawdzamy czy myszka jest na guziku
	void setButton(int w, int h, int X, int Y, std::string tekst, sf::Color t�o, int charSize); //ustawienie parmaetr�w guzika
	void moveBttn(int x, int y); //zmien pozycj� guzika
	void changeText(std::string tekst); //zmien tekst guzika
};