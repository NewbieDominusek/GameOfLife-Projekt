#include "kwadraty.h"
#include <SFML/Graphics.hpp>

void TileMap::updateLinie() {
	linie.setPrimitiveType(sf::Lines);
	linie.resize((width + 1) * 2 + (height + 1) * 2);

	int yOffset = (width + 1) * 2; //od którego indeksu w tablicy s¹ dane linii poziomych
	
	for (int x = 0; x < yOffset; x += 2) {			//Pionowe linie, przechodzi po parach vertexów w tablicy, x skacze o 2 wartoœci, wiêc trzeba x potem podzieliæ przy przypisywaniu pozycji do rysowania
		linie[x].position = sf::Vector2f(x / 2 * rozmiarX, 0 + marginTop);	//trzeba jeszcze uwzglêdniæ margines od góry
		linie[x + 1].position = sf::Vector2f(x / 2 * rozmiarX, height * rozmiarY + marginTop);
		linie[x].color = kolorLinii;
		linie[x + 1].color = kolorLinii;
	}

	for (int y = 0; y < (height + 1) * 2; y += 2) {		//Poziome linie, przechodzi po parach vertexów w tablicy, czêœæ tablicy dla linii poziomych jest po czêœci dla linii pionowych, dlatego jest + yOffset
		linie[y + yOffset].position = sf::Vector2f(0, y / 2 * rozmiarY + marginTop);		//trzeba jeszcze uwzglêdniæ margines od góry
		linie[y + 1 + yOffset].position = sf::Vector2f(width * rozmiarX, y / 2 * rozmiarY + marginTop);//trzeba jeszcze uwzglêdniæ margines od góry
		linie[y + yOffset].color = kolorLinii;
		linie[y + 1 + yOffset].color = kolorLinii;
	}
}

TileMap::TileMap(int* mapa, int w, int h, int wW, int wH) {
	width = w;
	height = h;
	rozmiarX = wW / width;	//obliczenie rozmiarów komórek dla dowolnej tablicy
	rozmiarY = (wH - marginTop - marginBottom) / height; //trzeba jeszcze uwzglêdniæ margines od góry i do³u
	map = mapa;
	oknoH = wH;
	oknoW = wW;
	origH = wH;
	origW = wW;

	kwadraty.setPrimitiveType(sf::Triangles);	//ustawiamy parametry tablicy z trójk¹tami rysuj¹cej komórki
	kwadraty.resize(height * width * 6);
	updateLinie();	//rysujemy linie
}

void TileMap::updateKwadraty() {	//aktualizacja planszy
	int numer;
	for (int x = 0; x < width; x++) {	//przechodzenie po tablicy
		for (int y = 0; y < height; y++)
		{
			numer = y * width + x;	//wartoœæ y po prostu przesuwa numer o d³ugoœæ tablicy razy y, wiêc otrzymujemy """2 wymiarow¹ tablicê"""
			sf::Vertex* trojkat = &kwadraty[numer * 6];	//tablica sk³ada siê z rozmiar * 6, bo na ka¿dy kwadrat potrzebne jest 2 trójk¹ty, czyli 6 punktów. Tutaj robimy uchwyt do tych trójk¹tów w grupach po 6
			trojkat[0].position = sf::Vector2f(x * rozmiarX, y * rozmiarY + marginTop);		//ustawiamy trójk¹ty z uwzglêdnieniem marginesu górnego
			trojkat[1].position = sf::Vector2f((x + 1) * rozmiarX, y * rozmiarY + marginTop);
			trojkat[2].position = sf::Vector2f(x * rozmiarX, (y + 1) * rozmiarY + marginTop);
			trojkat[3].position = sf::Vector2f(x * rozmiarX, (y + 1) * rozmiarY + marginTop);
			trojkat[4].position = sf::Vector2f((x + 1) * rozmiarX, y * rozmiarY + marginTop);
			trojkat[5].position = sf::Vector2f((x + 1) * rozmiarX, (y + 1) * rozmiarY + marginTop);

			if (map[numer] < 10) for(int i = 0; i <= 5; i++) trojkat[i].color = paletaKolor[map[numer]];	//ustawienie koloru trójk¹ta, kolor jest zale¿ny od wartoœci w odpowiadaj¹cym miejscu w tablicy g³ównej
		}
	}
}



void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {	//rysowanie planszy
	target.draw(kwadraty, states);
	if(drawBounds == YES) target.draw(linie, states);	//rysujemy podzia³kê jeœli jest w³¹czona
}


void TileMap::HandleEvent(sf::Event event) {	//handlowanie eventów
	float x, y;
	switch (event.type) {
	case sf::Event::MouseButtonPressed:	//naciœniêto myszkê
		if (event.mouseButton.y > marginTop / origH * oknoH && event.mouseButton.y < oknoH - marginBottom / origH * oknoH) {	//sprawdzamy czy naciœniêto na planszê (sprawdzamy tylko y, bo x jest na ca³¹ szerokoœæ i skalujemy do rozmiaru okna)
			x = event.mouseButton.x * origW / oknoW;	//skalujemy wartoœci x i y myszki tak, aby by³y w skali oryginalnego rozmiaru okna (jak zwiêkszymy okno to trzeba tê wartoœæ odpowiednio zmiejszyæ)
			y = event.mouseButton.y * origH / oknoH;	//wtedy siê fajnie dzieli

			x = x / rozmiarX;	//fajne dzielenie, jak mamy w pozycjê myszki zeskalowan¹ do oryginalnego rozmiaru okna, to mo¿emy podzieliæ przez oryginalny rozmiar komórek
			y = (y - marginTop) / rozmiarY;	//aby efektywnie otrzymaæ na któr¹ komórkê nacisneliœmy (sfml automatycznie skaluje renderowanie do okna, wiêc nie mogê zmieniæ rozmiarX i rozmiarY bo grafika siê rozjedzie)

			if (x > width || y > height) break;	//sprawdzamy czy wartoœci x i y s¹ w zakresie rozmiaru mapy, tak na wszelki wypadek
			showHolo = true; //jeœli jesteœmy w zakresie tablicy to pozwalamy na rysowanie hologramu (nie chcemy go rysowaæ jak nie ma myszki na tablicy bo to brzydko wygl¹da)

			numer = (int)y * width + (int)x; //obliczamy który index zmieniæ
			if (event.mouseButton.button == sf::Mouse::Left) {
				if (!pasteInEnabled) map[numer] = (map[numer] == 1) ? 0 : 1; //jeœli nie jest w³¹czone wklejanie, to po prostu prze³¹czamy stan komórki
				else {
					hologram(true); //jeœli w³¹czone by³o wklejanie, to wklejamy wzór
					pasteInEnabled = false; //wy³¹czamy wklejanie
				}
			}
			updateKwadraty();	//updateujemy rysowanie planszy
		}

		break;

	case sf::Event::Resized:	//ustalam nowe rozmiary okna
		oknoW = event.size.width;
		oknoH = event.size.height;
		break;

	case sf::Event::MouseMoved: //jeœli tylko ruszono myszk¹, robimy pocz¹tkowo to samo co przy naciœniêciu PPM
		if (!pasteInEnabled) break; //jeœli wklejanie jest wy³¹czone to nie ma sensu iœæ dalej, bo ten kod obs³uguje wyœwietlanie hologramu wzorku
		if (event.mouseMove.y > marginTop / origH * oknoH && event.mouseMove.y < oknoH - marginBottom / origH * oknoH) {	//sprawdzamy czy najechano na planszê (sprawdzamy tylko y, bo x jest na ca³¹ szerokoœæ i skalujemy do rozmiaru okna)
			x = event.mouseMove.x * origW / oknoW;	//skalujemy wartoœci x i y myszki tak, aby by³y w skali oryginalnego rozmiaru okna (jak zwiêkszymy okno to trzeba tê wartoœæ odpowiednio zmiejszyæ)
			y = event.mouseMove.y * origH / oknoH;	//wtedy siê fajnie dzieli

			x = x / rozmiarX;	//fajne dzielenie, jak mamy w pozycjê myszki zeskalowan¹ do oryginalnego rozmiaru okna, to mo¿emy podzieliæ przez oryginalny rozmiar komórek
			y = (y - marginTop) / rozmiarY;	//aby efektywnie otrzymaæ na któr¹ komórkê najechona (sfml automatycznie skaluje renderowanie do okna, wiêc nie mogê zmieniæ rozmiarX i rozmiarY bo grafika siê rozjedzie)

			if (x > width || y > height) break;	//sprawdzamy czy wartoœci x i y s¹ w zakresie rozmiaru mapy
			holoX = x; //ustawiamy koordynaty gdzie ma byæ wyœwietlany hologram
			holoY = y;
			showHolo = true; //pozwalamy na rysowanie hologramu

			hologram(false); //rysujemy hologram, ale go nie wstawiamy
			updateKwadraty();	//updateujemy rysowanie planszy
		}
		else if (showHolo) { //jeœli jesteœmy poza map¹ i rysowanie hologramu by³o w³¹czone
			showHolo = false; //wy³¹czamy rysowanie hologramu
			hologram(false); //wywo³anie aby usun¹æ pozosta³oœæ hologramu
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
	rozmiarX = origW / width;	//obliczenie rozmiarów komórek dla dowolnej tablicy
	rozmiarY = (origH - marginTop - marginBottom) / height; //trzeba jeszcze uwzglêdniæ margines od góry i do³u
	kwadraty.resize(height * width * 6);
	updateLinie();
	updateKwadraty();
}

void TileMap::hologram(bool clicked) {
	for (int i = 0; i < height * width; i++) map[i] = map[i] % 2; //ustaw planszê do stanu z danymi 0 lub 1

	if (!showHolo) return; //sprawdŸ czy mamy w ogóle wyœwietliæ hologram
	if (holoWidth > width || holoHeight > height) return; //czy hologram mieœci siê w planszy
	
	//jeœli wszystko siê zgadza

	for (int row = 0; row < holoHeight; row++) { //przechodzimy po hologramie
		for (int col = 0; col < holoWidth; col++) {
			if (!zawijanie && ((row + holoY >= height) || (col + holoX >= width))) continue; //jeœli wyst¹pi³o zawijanie a mamy je wy³¹czone, to pomiñ komórkê
			int num = ((row + holoY >= height) ? row + holoY - height : row + holoY) * width + ((col + holoX >= width) ? col + holoX - width : col + holoX); //liczymy numer komórki (pozycja myszki + miejsce w hologramie) i uwzglêdniamy zawijanie
			if (clicked) map[num] = holoData[row * holoWidth + col]; //jeœli klikniêto, to zamiast hologramu wklej wzór do tablicy g³ównej

			//inaczej rysuj hologram:
			else if (map[num] == 0) map[num] = (holoData[row * holoWidth + col] == 0) ? 8 : 6;	//jeœli komórka jest martwa
			else map[num] = (holoData[row * holoWidth + col] == 0) ? 7 : 9;		//jeœli jest ¿ywa
		}
	}
}