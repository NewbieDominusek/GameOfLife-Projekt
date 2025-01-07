#include "kwadraty.h"
#include <SFML/Graphics.hpp>

void TileMap::updateLinie() {
	linie.setPrimitiveType(sf::Lines);
	linie.resize((width + 1) * 2 + (height + 1) * 2);

	int yOffset = (width + 1) * 2; //przesuni�cie do cz�ci tablicy linii wyznaczonej dla linii poziomych
	
	for (int x = 0; x < yOffset; x += 2) {			//Pionowe linie, przechodzi po parach vertex�w w tablicy, x skacze o 2 warto�ci, wi�c trzeba x potem podzieli� przy przypisywaniu pozycji
		linie[x].position = sf::Vector2f(x / 2 * rozmiarX, 0 + marginTop);	//trzeba jeszcze uwzgl�dni� margines od g�ry
		linie[x + 1].position = sf::Vector2f(x / 2 * rozmiarX, height * rozmiarY + marginTop);
		linie[x].color = kolorLinii;
		linie[x + 1].color = kolorLinii;
	}

	for (int y = 0; y < (height + 1) * 2; y += 2) {		//Poziome linie, przechodzi po parach vertex�w w tablicy, cz�� tablicy dla linii poziomych jest po cz�ci dla linii pionowych, dlatego jest + yOffset
		linie[y + yOffset].position = sf::Vector2f(0, y / 2 * rozmiarY + marginTop);		//trzeba jeszcze uwzgl�dni� margines od g�ry
		linie[y + 1 + yOffset].position = sf::Vector2f(width * rozmiarX, y / 2 * rozmiarY + marginTop);//trzeba jeszcze uwzgl�dni� margines od g�ry
		linie[y + yOffset].color = kolorLinii;
		linie[y + 1 + yOffset].color = kolorLinii;
	}
}

TileMap::TileMap(int* mapa, int w, int h, int wW, int wH) {
	width = w;
	height = h;
	rozmiarX = wW / width;	//obliczenie rozmiar�w kom�rek dla dowolnej tablicy
	rozmiarY = (wH - marginTop - marginBottom) / height; //trzeba jeszcze uwzgl�dni� margines od g�ry i do�u
	map = mapa;
	oknoH = wH;
	oknoW = wW;
	origH = wH;
	origW = wW;

	kwadraty.setPrimitiveType(sf::Triangles);	//ustawiamy parametry tablicy z tr�jk�tami rysuj�cej kom�rki
	kwadraty.resize(height * width * 6);
	updateLinie();	//rysujemy linie
}

void TileMap::updateKwadraty() {	//aktualizacja kwadrat�w
	int numer;
	for (int x = 0; x < width; x++) {	//przechodzenie po tablicy
		for (int y = 0; y < height; y++)
		{
			numer = y * width + x;	//warto�� y po prostu przesuwa numer o d�ugo�� tablicy razy y, wi�c otrzymujemy """2 wymiarow� tablic�"""
			sf::Vertex* trojkat = &kwadraty[numer * 6];	//tablica sk�ada si� z rozmiar * 6, bo na ka�dy kwadrat potrzebne jest 2 tr�jk�ty, czyli 6 punkt�w. Tutaj robimy uchwyt do tych tr�jk�t�w w grupach po 6
			trojkat[0].position = sf::Vector2f(x * rozmiarX, y * rozmiarY + marginTop);		//ustawiamy tr�jk�ty z uwzgl�dnieniem marginesu g�rnego
			trojkat[1].position = sf::Vector2f((x + 1) * rozmiarX, y * rozmiarY + marginTop);
			trojkat[2].position = sf::Vector2f(x * rozmiarX, (y + 1) * rozmiarY + marginTop);
			trojkat[3].position = sf::Vector2f(x * rozmiarX, (y + 1) * rozmiarY + marginTop);
			trojkat[4].position = sf::Vector2f((x + 1) * rozmiarX, y * rozmiarY + marginTop);
			trojkat[5].position = sf::Vector2f((x + 1) * rozmiarX, (y + 1) * rozmiarY + marginTop);


			if (map[numer] <= 7) for(int i = 0; i <= 5; i++) trojkat[i].color = paletaKolor[map[numer]];	//kolorki :3 UwU (kolorek = warto�� w indeksie)

			
		}	//I am losing my mind
	}
}



void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {	//rysowanie planszy
	target.draw(kwadraty, states);
	if(drawBounds == YES) target.draw(linie, states);	//rysujemy podzia�k� je�li jest w��czona
}


void TileMap::HandleEvent(sf::Event event) {	//handlowanie event�w
	float x, y;
	switch (event.type) {
	case sf::Event::MouseButtonPressed:	//naci�ni�to myszk�
		if (event.mouseButton.y > marginTop / origH * oknoH && event.mouseButton.y < oknoH - marginBottom / origH * oknoH) {	//sprawdzamy czy naci�ni�to na plansz� (sprawdzamy tylko y, bo x jest na ca�� szeroko�� i skalujemy do rozmiaru okna)
			x = event.mouseButton.x * origW / oknoW;	//skalujemy warto�ci x i y myszki tak, aby by�y w skali oryginalnego rozmiaru okna (jak zwi�kszymy okno to trzeba t� warto�� odpowiednio zmiejszy�)
			y = event.mouseButton.y * origH / oknoH;	//wtedy si� fajnie dzieli trust me bro pls bro

			x = x / rozmiarX;	//fajne dzielenie, jak mamy w pozycj� myszki zeskalowan� do oryginalnego rozmiaru okna, to mo�emy podzieli� przez oryginalny rozmiar kom�rek
			y = (y - marginTop) / rozmiarY;	//aby efektywnie otrzyma� na kt�r� kom�rk� nacisneli�my (sfml automatycznie skaluje renderowanie do okna, wi�c nie mog� zmieni� rozmiarX i rozmiarY bo grafika si� rozjedzie)

			if (x > width || y > height) break;	//sprawdzamy czy warto�ci x i y s� w zakresie rozmiaru mapy
			showHolo = true;

			numer = (int)y * width + (int)x; //obliczamy kt�ry index tentegesowa�
			if (event.mouseButton.button == sf::Mouse::Left) {
				if (!pasteInEnabled) map[numer] = (map[numer] == 1) ? 0 : 1;	//zmieniamy warto�� w indeksie w zale�no�ci od przycisku myszki
				else {
					hologram(true);
					pasteInEnabled = false;
				}
			}
			updateKwadraty();	//updateujemy rysowanie planszy
		}

		break;

	case sf::Event::Resized:	//ustalam nowe rozmiary okna
		oknoW = event.size.width;
		oknoH = event.size.height;
		break;

	case sf::Event::MouseMoved: //je�li tylko ruszono myszk�, robimy pocz�tkowo to samo co przy naci�ni�ciu PPM
		if (!pasteInEnabled) break;
		if (event.mouseMove.y > marginTop / origH * oknoH && event.mouseMove.y < oknoH - marginBottom / origH * oknoH) {	//sprawdzamy czy naci�ni�to na plansz� (sprawdzamy tylko y, bo x jest na ca�� szeroko�� i skalujemy do rozmiaru okna)
			x = event.mouseMove.x * origW / oknoW;	//skalujemy warto�ci x i y myszki tak, aby by�y w skali oryginalnego rozmiaru okna (jak zwi�kszymy okno to trzeba t� warto�� odpowiednio zmiejszy�)
			y = event.mouseMove.y * origH / oknoH;	//wtedy si� fajnie dzieli trust me bro pls bro

			x = x / rozmiarX;	//fajne dzielenie, jak mamy w pozycj� myszki zeskalowan� do oryginalnego rozmiaru okna, to mo�emy podzieli� przez oryginalny rozmiar kom�rek
			y = (y - marginTop) / rozmiarY;	//aby efektywnie otrzyma� na kt�r� kom�rk� nacisneli�my (sfml automatycznie skaluje renderowanie do okna, wi�c nie mog� zmieni� rozmiarX i rozmiarY bo grafika si� rozjedzie)

			if (x > width || y > height) break;	//sprawdzamy czy warto�ci x i y s� w zakresie rozmiaru mapy
			holoX = x;
			holoY = y;
			showHolo = true;

			hologram(false);
			updateKwadraty();	//updateujemy rysowanie planszy
		}
		else if (showHolo) {
			showHolo = false;
			hologram(false); //wywo�anie aby usun�� pozosta�o�� hologramu
			updateKwadraty();	//updateujemy rysowanie planszy
		}
		break;

	default:
		break;
	}
}

void TileMap::resizePlansza(int w, int h, int* nowaMapa) {
	map = nowaMapa;
	width = w;
	height = h;
	rozmiarX = origW / width;	//obliczenie rozmiar�w kom�rek dla dowolnej tablicy
	rozmiarY = (origH - marginTop - marginBottom) / height; //trzeba jeszcze uwzgl�dni� margines od g�ry i do�u
	kwadraty.resize(height * width * 6);
	updateLinie();
	updateKwadraty();
}

void TileMap::hologram(bool clicked) {
	for (int i = 0; i < height * width; i++) map[i] = map[i] % 2; //usu� wcze�niejszy hologram

	if (!showHolo) return; //sprawd� czy mamy wy�wietli� hologram
	if (holoWidth > width || holoHeight > height) return; //czy hologram mie�ci si� w planszy
	

	for (int row = 0; row < holoHeight; row++) { //przechodzimy po hologramie
		for (int col = 0; col < holoWidth; col++) {
			int num = ((row + holoY >= height) ? row + holoY - height : row + holoY) * width + ((col + holoX >= width) ? col + holoX - width : col + holoX); //liczymy numer kom�rki (pozycja myszki + miejsce w hologramie) i uwzgl�dniamy zawijanie
			if (clicked) map[num] = holoData[row * holoWidth + col]; //je�li klikni�to, to zamist hologramu po prostu narysuj wz�r

			//inaczej rysuj hologram:
			else if (map[num] == 0) map[num] = (holoData[row * holoWidth + col] == 0) ? 6 : 4;	//je�li kom�rka jest martwa
			else map[num] = (holoData[row * holoWidth + col] == 0) ? 5 : 7;					//je�li jest �ywa
		}
	}
}