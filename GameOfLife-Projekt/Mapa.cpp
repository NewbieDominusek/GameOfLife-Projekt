#include "Mapa.h"


void Mapa::resetMapa() { //wyzeruj ca�� map�
	for (int i = 0; i < width * height; i++) {
		map[i] = 0; //przejd� po ca�ej mapie i wyzeruj
	}
}

void Mapa::resizePlansza(int deltaWidth, int deltaHeight) { //zmiana rozmiaru g��wnej tablicy mapy
	int newWidth = width + deltaWidth, newHeight = height + deltaHeight;
	if (newWidth < 3) newWidth = 3;
	if (newHeight < 3) newHeight = 3;
	if (newWidth == width && newHeight == height) return;
	int *temp = new int[newWidth * newHeight]; //tymczasowa tablica do przechowania oryginalnych warto�ci kom�rek (rozmiar docelowej tablicy)
	for (int i = 0; i < newWidth * newHeight; i++) temp[i] = 0;
	int minW = (width >= newWidth) ? newWidth : width; //ustal kt�re wymiary s� mniejsze aby nie wyj�� poza rozmiar tablic
	int minH = (height >= newHeight) ? newHeight : height; //ustal kt�re wymiary s� mniejsze aby nie wyj�� poza rozmiar tablic
	for (int x = 0; x < minW; x++) { //[rzechodzimy po kom�rkach
		for (int y = 0; y < minH; y++) {
			temp[y * newWidth + x] = map[y * width + x]; //przekopiuj kom�rki tablicy g��wnej do tymczasowej, je�li tymczasowa jest mniejsza to dane z g��wnej zostan� uci�te
		}
	}

	delete []map; //usu� g��wn� tablic�
	map = new int[newWidth * newHeight]; //i stw�rz od nowa o nowych rozmiarach
	for (int x = 0; x < newWidth; x++) {
		for (int y = 0; y < newHeight; y++) {
			map[y * newWidth + x] = temp[y * newWidth + x]; //wklej dane z tablicy tymczasowej do g��wnej
		}
	}
	delete []temp; //zwolnij tymczasow�
	height = newHeight; //ustaw nowy rozmiar
	width = newWidth;
}

//parzyste liczby to martwa kom�rka, nieparzyste to �ywa
//0 - martwa
//1 - �ywa
//2 - sprawdzona martwa
//3 - sprawdzona �ywa
//4 - nowa martwa
//5 - nowa �ywa
// 
// hologramy:
//6 - martwa rysowana jako �ywa
//7 - �ywa rysowana jako martwa
//8 - martwa rysowana innym kolorem
//9 - �ywa rysowana innym kolorem

// <2 s�siad�w umiera
//2-3 s�siad�w prze�ywa
// >3 s�siad�w umiera
//  3 s�siad�w o�ywa


void Mapa::calculateMapa() { //g��wna funkcja gry obliczaj�ca nast�pn� iteracj� planszy
	int liveNeighbourCount = 0; //�ywi s�siedzi
	for (int cellNum = 0; cellNum < width * height; cellNum++) { //przechodzimy po ca�ej mapie
		if (map[cellNum] > 5) map[cellNum] = map[cellNum] % 2; //usuwamy kom�rki pomalowane przez funkcj� hologramow�
		if (map[cellNum] != 1) continue; //obliczamy tylko dla �ywych kom�rek, kt�re nie by�y jeszcze liczone (optymalizacja)
		map[cellNum] = decideCellState(countNeighbours(cellNum), map[cellNum]); //liczymy s�siad�w obecnej kom�rki
		calculateNeighbours(cellNum); //teraz obliczamy s�siad�w obecnej kom�rki (optymalizacja, obliczamy tylko �ywe kom�rki i ich s�siad�w, bo martwa kom�rka z martwymi s�siadami nie mo�e zmieni� stanu)
	}

	for (int i = 0; i < height * width; i++) map[i] = map[i] % 2; //usu� robocze warto�ci kom�rek (obliczona tablica ma zawierac tylko 1 lub 0)
}

int* Mapa::generateNeighbourNums(int cellNum){
	int row = cellNum / width;
	int col = cellNum % width;

	int above = (row == 0) ? ((zawijanie) ? height - 1 : -1) : row - 1; //rz�d nad kom�rk�, je�li rz�d == 0 i mamy zawija� to bierzemy najni�szy rz�d, je�li nie mamy zawija� to ustawiamy na -1
	int below = (row == height - 1) ? ((zawijanie) ? 0 : -1) : row + 1; //rz�d pod kom�rk�, je�li rz�d == height - 1 i mamy zawija� to bierzemy rz�d == 0, je�li nie mamy zawija� to ustawiamy na -1
	int left = (col == 0) ? ((zawijanie) ? width - 1 : -1) : col - 1; //kolumna na lewo od kom�rki, je�li kolumna == 0 i mamy zawija� to bierzemy kolumna == width - 1, je�li nie mamy zawija� to ustawiamy na -1
	int right = (col == width - 1) ? ((zawijanie) ? 0 : -1) : col + 1; //kolumna na prawo od kom�rki, je�li kolumna == width - 1 i mamy zawija� to bierzemy kolumna == 0, je�li nie mamy zawija� to ustawiamy na -1

	int* coords = new int[16]; //koordynaty kom�rek do sprawdzenia
	coords[0] = above; coords[1] = left;
	coords[2] = above; coords[3] = col;
	coords[4] = above; coords[5] = right;
	coords[6] = row; coords[7] = left;
	coords[8] = row; coords[9] = right;
	coords[10] = below;	coords[11] = left;
	coords[12] = below;	coords[13] = col;
	coords[14] = below;	coords[15] = right;
	return coords;
}

int Mapa::decideCellState(int neighbourCnt, int currentState) { //funkcja decyduj�ca nast�pny stan kom�rki na podstawie liczby s�siad�w
	if (neighbourCnt < 2) return (currentState == 1) ? 4 : 2;	//ma za ma�o s�siad�w i jest �ywa, to ma um�e�		(nowa martwa : sprawdzona martwa)
	else if (neighbourCnt > 3) return (currentState == 1) ? 4 : 2;	//ma za du�o s�siad�w i jest �ywa to ma um�e�	(nowa martwa : sprawdzona martwa)
	else if (neighbourCnt == 3) return (currentState == 0) ? 5 : 3;	//ma 3 s�siad�w i jest martwa to ma o�y�		(nowa �ywa : sprawdzona �ywa)
	else return (currentState == 0) ? 2 : 3; //stan ma pozosta� niezmieniony		(sprawdzona martwa : sprawdzona �ywa)
}

void Mapa::calculateNeighbours(int cellNum) { //oblicz s�siad�w podanej kom�rki
	int* coords = generateNeighbourNums(cellNum);

	for (int i = 0; i < 16; i += 2) { //przejd� po kom�rkach do sprawdzenia
		if (coords[i] < 0 || coords[i + 1] < 0) continue; //je�li kt�ra� warto�� jest ujemna, to znaczy �e nie mamy zawija� i dana kom�rka wychodzi poza plansz�, nale�y pomin��
		int num = coords[i] * width + coords[i + 1]; //numer kom�rki
		if (map[num] > 5) map[num] = map[num] % 2; //usuwamy warto�ci robocze ustawione przez hologram
		if (map[num] != 0) continue; //obliczamy tylko martwe niesprawdzone kom�rki (�ywe zostan� obliczone przez p�tl� g��wnej funkcji)
		map[num] = decideCellState(countNeighbours(num), map[num]); //oblicz nowy stan kom�rki
	}
	delete[]coords;
}

int Mapa::countNeighbours(int cellNum) { //policz �ywych s�siad�w kom�rki
	int count = 0;
	int* coords = generateNeighbourNums(cellNum);

	for (int i = 0; i < 16; i += 2) {
		if (coords[i] < 0 || coords[i + 1] < 0) continue; //je�li kt�ra� warto�� jest ujemna, to znaczy �e nie mamy zawija� i dana kom�rka wychodzi poza plansz�, nale�y pomin��
		int num = coords[i] * width + coords[i + 1]; //numer kom�rki
		if (map[num] > 5) map[num] = map[num] % 2;//usuwamy pomalowane kom�rki
		if (map[num] == 1 || map[num] == 3 || map[num] == 4) count++; //�ywa / sprawdzona �ywa / nowa martwa (by�a �ywa)
	}
	return count;
}

Mapa::Mapa() {
	zawijanie = true;
	height = 40;
	width = 64;
	map = new int[height * width];
	for (int i = 0; i < height * width; i++) map[i] = 0;
}

Mapa::Mapa(int setWidth, int setHeight) {
	zawijanie = true;
	height = setHeight;
	width = setWidth;
	map = new int[height * width];
	for (int i = 0; i < height * width; i++) map[i] = 0;
}

Mapa::~Mapa(){
	delete[]map;
}
