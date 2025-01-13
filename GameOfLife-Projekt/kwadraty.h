#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class TileMap : public sf::Drawable
{
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const; //potrzebne aby mo�na by�o bezpo�rednio przekaza� obiekt do window.draw()
	const float marginTop = 30.0f, marginBottom = 65.0f;	//margines od g�ry i do�u
	int height, width, numer;	//Wymiary tablicy i zmienna pomocnicza
	float rozmiarX, rozmiarY;		//rozmiar kom�rek do renderowania, ustawiane przy tworzeniu obiektu (konstruktor) i si� nie zmienia
	float oknoW, oknoH, origW, origH;	//rozmiar obecny okna i rozmiar oryginaly okna (potrzebne do skalowania)
	int* map;	//Podstawowa tablica gry z martwymi/�ywymi kom�rkami
	int holoX, holoY; //ostatnia pozycja hologramu do narysowania
	sf::VertexArray kwadraty;	//wektor z parami tr�jk�t�w do renderowania kom�rek
	sf::VertexArray linie;	//wektor z liniami mi�dzy kwadratami
	sf::Color paletaKolor[10] = {sf::Color::Black, sf::Color::White, sf::Color::Black, sf::Color::White, sf::Color::Black, sf::Color::White,sf::Color::Yellow, sf::Color::Blue, sf::Color::Blue , sf::Color::Yellow }; //tablica z kolorami kom�rki


public:
	bool zawijanie = true;
	bool pasteInEnabled = false; //czy jest aktywne wklejanie wzoru
	bool showHolo = false; //czy ma by� wy�wietlany wz�r
	std::vector<int> holoData; //dane o hologramie
	int holoWidth, holoHeight; //dane o rozmiarze hologramu, musi by� kwadrat;
	enum DrawOutline //czy rysowa� linie mi�dzy kom�rkami
	{
		YES,
		NO
	};
	DrawOutline drawBounds{ NO };//czy rysowa� linie mi�dzy kom�rkami
	sf::Color kolorLinii{ sf::Color::Magenta }; //kolor linii mi�dzy kom�rkami
	void updateKwadraty();				//Aktualizacja tablicy do renderowania na podstawie podstatowej tablicy gry
	void updateLinie();			//aktualizacja linii mi�dzy kom�rkami
	TileMap(int* mapa, int w, int h, int wW, int wH);	//konstruktor, trzeba da� tablic� z kom�rkami, rozmiary tablicy oraz rozmiary okna
	void HandleEvent(sf::Event event);	//handlowanie event�w
	void resizePlansza(int w, int h, int* nowaMapa);  //aktualizacja ryzowanej planszy przy zmianie rozmiaru tablicy g��wnej
	void hologram(bool clicked); //rysowanie/wklejanie hologramu (je�li przekazane true to wklei wz�r)
};

