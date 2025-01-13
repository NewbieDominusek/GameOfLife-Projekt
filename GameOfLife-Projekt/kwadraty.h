#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class TileMap : public sf::Drawable
{
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const; //potrzebne aby mo¿na by³o bezpoœrednio przekazaæ obiekt do window.draw()
	const float marginTop = 30.0f, marginBottom = 65.0f;	//margines od góry i do³u
	int height, width, numer;	//Wymiary tablicy i zmienna pomocnicza
	float rozmiarX, rozmiarY;		//rozmiar komórek do renderowania, ustawiane przy tworzeniu obiektu (konstruktor) i siê nie zmienia
	float oknoW, oknoH, origW, origH;	//rozmiar obecny okna i rozmiar oryginaly okna (potrzebne do skalowania)
	int* map;	//Podstawowa tablica gry z martwymi/¿ywymi komórkami
	int holoX, holoY; //ostatnia pozycja hologramu do narysowania
	sf::VertexArray kwadraty;	//wektor z parami trójk¹tów do renderowania komórek
	sf::VertexArray linie;	//wektor z liniami miêdzy kwadratami
	sf::Color paletaKolor[10] = {sf::Color::Black, sf::Color::White, sf::Color::Black, sf::Color::White, sf::Color::Black, sf::Color::White,sf::Color::Yellow, sf::Color::Blue, sf::Color::Blue , sf::Color::Yellow }; //tablica z kolorami komórki


public:
	bool zawijanie = true;
	bool pasteInEnabled = false; //czy jest aktywne wklejanie wzoru
	bool showHolo = false; //czy ma byæ wyœwietlany wzór
	std::vector<int> holoData; //dane o hologramie
	int holoWidth, holoHeight; //dane o rozmiarze hologramu, musi byæ kwadrat;
	enum DrawOutline //czy rysowaæ linie miêdzy komórkami
	{
		YES,
		NO
	};
	DrawOutline drawBounds{ NO };//czy rysowaæ linie miêdzy komórkami
	sf::Color kolorLinii{ sf::Color::Magenta }; //kolor linii miêdzy komórkami
	void updateKwadraty();				//Aktualizacja tablicy do renderowania na podstawie podstatowej tablicy gry
	void updateLinie();			//aktualizacja linii miêdzy komórkami
	TileMap(int* mapa, int w, int h, int wW, int wH);	//konstruktor, trzeba daæ tablicê z komórkami, rozmiary tablicy oraz rozmiary okna
	void HandleEvent(sf::Event event);	//handlowanie eventów
	void resizePlansza(int w, int h, int* nowaMapa);  //aktualizacja ryzowanej planszy przy zmianie rozmiaru tablicy g³ównej
	void hologram(bool clicked); //rysowanie/wklejanie hologramu (jeœli przekazane true to wklei wzór)
};

