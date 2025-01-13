#include "Button.h"
#include <SFML/Graphics.hpp>
#include <iostream>

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {	//renderowanie guzika i napisu
	target.draw(rectangle, states);
	target.draw(text, states);
}

int Button::mouseOnButton(int x, int y) {
	if (x > posX && x < posX + width)	//sprawdzenie czy pozycja myszki mie�ci wi� w X guzika
		if (y > posY && y < posY + height)	//sprawdzenie czy pozycja myszki mie�ci wi� w Y guzika
			return 1;
	return 0;
}

void Button::setButton(int w, int h, int X, int Y, std::string tekst, sf::Color t�o, int charSize) {
	width = w;
	height = h;
	posX = X;
	posY = Y;
	if (!czcionka.loadFromFile("kenyan_coffee_rg.otf")) std::cout << "czcionka nie dziala" << std::endl;  //sprawdzamy za�adowanie czcionki (nawet jak nie za�aduje to progrma mo�e dzia�a� tylko nie ma tekstu)
	text.setFont(czcionka); //ustawiamy czcionk�
	text.setString(tekst); //ustawiamy tekst
	text.setPosition(sf::Vector2f(posX, posY)); //ustawiamy pozycj�
	text.setCharacterSize(charSize); //ustawiamy rozmiar znak�w
	text.setFillColor(sf::Color::Black); //ustawiamy kolor
	rectangle.setSize(sf::Vector2f(width, height)); //rozmiar guzika
	rectangle.setPosition(sf::Vector2f(posX, posY)); //pozycje guzika
	rectangle.setFillColor(t�o); //kolor t�a
}

void Button::moveBttn(int x, int y) { //przestaw guzik
	rectangle.setPosition(sf::Vector2f(x, y));
}

void Button::changeText(std::string tekst) { //zmie� tekst na guziku
	text.setString(tekst);
}

void Button::changeColor(sf::Color nowyKolor) {
	rectangle.setFillColor(nowyKolor);
}