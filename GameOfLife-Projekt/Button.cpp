#include "Button.h"
#include <SFML/Graphics.hpp>
#include <iostream>

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {	//renderowanie guzika i napisu
	target.draw(rectangle, states);
	target.draw(text, states);
}

int Button::mouseOnButton(int x, int y) {
	if (x > posX && x < posX + width)	//sprawdzenie czy pozycja myszki mieœci wiê w X guzika
		if (y > posY && y < posY + height)	//sprawdzenie czy pozycja myszki mieœci wiê w Y guzika
			return 1;
	return 0;
}

void Button::setButton(int w, int h, int X, int Y, std::string tekst, sf::Color t³o, int charSize) {
	width = w;
	height = h;
	posX = X;
	posY = Y;
	if (!czcionka.loadFromFile("kenyan_coffee_rg.otf")) std::cout << "Chuj nie czcionka" << std::endl;  //skill issue
	text.setFont(czcionka);
	text.setString(tekst);
	text.setPosition(sf::Vector2f(posX, posY));
	text.setCharacterSize(charSize);
	text.setFillColor(sf::Color::Black);
	rectangle.setSize(sf::Vector2f(width, height));
	rectangle.setPosition(sf::Vector2f(posX, posY));
	rectangle.setFillColor(t³o);
}

void Button::moveBttn(int x, int y) {
	rectangle.setPosition(sf::Vector2f(x, y));
}

void Button::changeText(std::string tekst) {
	text.setString(tekst);
}