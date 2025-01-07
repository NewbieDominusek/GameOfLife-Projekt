#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class TileMap : public sf::Drawable
{
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const; //potrzebne aby mo¿na by³o bezpoœrednio przekazaæ obiekt do window.draw()
	const float marginTop = 30.0f, marginBottom = 65.0f;	//margines od góry i do³u
	int height, width, numer;	//Wymiary tablicy i zmienna pomocnicza
	float rozmiarX, rozmiarY;		//rozmiar komórek do renderowania, ustawiane przy tworzeniu obiektu (konstruktor) i siê nie zmienia
	float oknoW, oknoH, origW, origH;	//rozmiar obecny okna i rozmiar oryginaly okna (potrzebne trust me bro)
	int* map;							//Podstawowa tablica gry z martwymi/¿ywymi komórkami
	int holoX, holoY; //ostatnia pozycja hologramu do narysowania
	sf::VertexArray kwadraty;			//Tablica z parami trójk¹tów do renderowania komórek
	sf::VertexArray linie;				//Tablica z liniami miêdzy kwadratami
	sf::Color paletaKolor[8] = {sf::Color::Black, sf::Color::White, sf::Color::Black, sf::Color::White,sf::Color::Yellow, sf::Color::Blue, sf::Color::Blue , sf::Color::Yellow }; //jakie kolory mo¿e mieæ komórka


public:
	bool pasteInEnabled = false, showHolo = false;
	std::vector<int> holoData; //dane o hologramie
	int holoWidth, holoHeight; //dane o rozmiarze hologramu, musi byæ kwadrat;
	enum DrawOutline //czy rysowaæ linie miêdzy komórkami
	{
		YES,
		NO
	};
	DrawOutline drawBounds{ NO };//czy rysowaæ linie miêdzy komórkami
	sf::Color kolorLinii{ sf::Color::Magenta };
	void updateKwadraty();				//Aktualizacja tablicy do renderowania na podstawie podstatowej tablicy gry
	void updateLinie();			//aktualizacja linii miêdzy komórkami (narysowanie ich)
	TileMap(int* mapa, int w, int h, int wW, int wH);	//konstruktor, trzeba daæ tablicê z komórkami, rozmiary tablicy oraz rozmiary okna
	void HandleEvent(sf::Event event);	//handlowanie eventów
	void resizePlansza(int w, int h, int* nowaMapa);  //ustawienie wartoœci przy zmianie rozmiaru okna
	void hologram(bool clicked);
};

