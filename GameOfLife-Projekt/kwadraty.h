#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class TileMap : public sf::Drawable
{
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const; //potrzebne aby mo�na by�o bezpo�rednio przekaza� obiekt do window.draw()
	const float marginTop = 30.0f, marginBottom = 65.0f;	//margines od g�ry i do�u
	int height, width, numer;	//Wymiary tablicy i zmienna pomocnicza
	float rozmiarX, rozmiarY;		//rozmiar kom�rek do renderowania, ustawiane przy tworzeniu obiektu (konstruktor) i si� nie zmienia
	float oknoW, oknoH, origW, origH;	//rozmiar obecny okna i rozmiar oryginaly okna (potrzebne trust me bro)
	int* map;							//Podstawowa tablica gry z martwymi/�ywymi kom�rkami
	int holoX, holoY; //ostatnia pozycja hologramu do narysowania
	sf::VertexArray kwadraty;			//Tablica z parami tr�jk�t�w do renderowania kom�rek
	sf::VertexArray linie;				//Tablica z liniami mi�dzy kwadratami
	sf::Color paletaKolor[8] = {sf::Color::Black, sf::Color::White, sf::Color::Black, sf::Color::White,sf::Color::Yellow, sf::Color::Blue, sf::Color::Blue , sf::Color::Yellow }; //jakie kolory mo�e mie� kom�rka


public:
	bool pasteInEnabled = false, showHolo = false;
	std::vector<int> holoData; //dane o hologramie
	int holoWidth, holoHeight; //dane o rozmiarze hologramu, musi by� kwadrat;
	enum DrawOutline //czy rysowa� linie mi�dzy kom�rkami
	{
		YES,
		NO
	};
	DrawOutline drawBounds{ NO };//czy rysowa� linie mi�dzy kom�rkami
	sf::Color kolorLinii{ sf::Color::Magenta };
	void updateKwadraty();				//Aktualizacja tablicy do renderowania na podstawie podstatowej tablicy gry
	void updateLinie();			//aktualizacja linii mi�dzy kom�rkami (narysowanie ich)
	TileMap(int* mapa, int w, int h, int wW, int wH);	//konstruktor, trzeba da� tablic� z kom�rkami, rozmiary tablicy oraz rozmiary okna
	void HandleEvent(sf::Event event);	//handlowanie event�w
	void resizePlansza(int w, int h, int* nowaMapa);  //ustawienie warto�ci przy zmianie rozmiaru okna
	void hologram(bool clicked);
};

