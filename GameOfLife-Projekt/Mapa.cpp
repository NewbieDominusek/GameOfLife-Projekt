#include "Mapa.h"


void Mapa::resetMapa() { //wyzeruj ca³¹ mapê
	for (int i = 0; i < width * height; i++) {
		map[i] = 0; //przejdŸ po ca³ej mapie i wyzeruj
	}
}

void Mapa::resizePlansza(int deltaWidth, int deltaHeight) { //zmiana rozmiaru g³ównej tablicy mapy
	int newWidth = width + deltaWidth, newHeight = height + deltaHeight;
	if (newWidth < 3) newWidth = 3;
	if (newHeight < 3) newHeight = 3;
	if (newWidth == width && newHeight == height) return;
	int *temp = new int[newWidth * newHeight]; //tymczasowa tablica do przechowania oryginalnych wartoœci komórek (rozmiar docelowej tablicy)
	for (int i = 0; i < newWidth * newHeight; i++) temp[i] = 0;
	int minW = (width >= newWidth) ? newWidth : width; //ustal które wymiary s¹ mniejsze aby nie wyjœæ poza rozmiar tablic
	int minH = (height >= newHeight) ? newHeight : height; //ustal które wymiary s¹ mniejsze aby nie wyjœæ poza rozmiar tablic
	for (int x = 0; x < minW; x++) { //[rzechodzimy po komórkach
		for (int y = 0; y < minH; y++) {
			temp[y * newWidth + x] = map[y * width + x]; //przekopiuj komórki tablicy g³ównej do tymczasowej, jeœli tymczasowa jest mniejsza to dane z g³ównej zostan¹ uciête
		}
	}

	delete []map; //usuñ g³ówn¹ tablicê
	map = new int[newWidth * newHeight]; //i stwórz od nowa o nowych rozmiarach
	for (int x = 0; x < newWidth; x++) {
		for (int y = 0; y < newHeight; y++) {
			map[y * newWidth + x] = temp[y * newWidth + x]; //wklej dane z tablicy tymczasowej do g³ównej
		}
	}
	delete []temp; //zwolnij tymczasow¹
	height = newHeight; //ustaw nowy rozmiar
	width = newWidth;
}

//parzyste liczby to martwa komórka, nieparzyste to ¿ywa
//0 - martwa
//1 - ¿ywa
//2 - sprawdzona martwa
//3 - sprawdzona ¿ywa
//4 - nowa martwa
//5 - nowa ¿ywa
// 
// hologramy:
//6 - martwa rysowana jako ¿ywa
//7 - ¿ywa rysowana jako martwa
//8 - martwa rysowana innym kolorem
//9 - ¿ywa rysowana innym kolorem

// <2 s¹siadów umiera
//2-3 s¹siadów prze¿ywa
// >3 s¹siadów umiera
//  3 s¹siadów o¿ywa


void Mapa::calculateMapa() { //g³ówna funkcja gry obliczaj¹ca nastêpn¹ iteracjê planszy
	int liveNeighbourCount = 0; //¿ywi s¹siedzi
	for (int cellNum = 0; cellNum < width * height; cellNum++) { //przechodzimy po ca³ej mapie
		if (map[cellNum] > 5) map[cellNum] = map[cellNum] % 2; //usuwamy komórki pomalowane przez funkcjê hologramow¹
		if (map[cellNum] != 1) continue; //obliczamy tylko dla ¿ywych komórek, które nie by³y jeszcze liczone (optymalizacja)
		map[cellNum] = decideCellState(countNeighbours(cellNum), map[cellNum]); //liczymy s¹siadów obecnej komórki
		calculateNeighbours(cellNum); //teraz obliczamy s¹siadów obecnej komórki (optymalizacja, obliczamy tylko ¿ywe komórki i ich s¹siadów, bo martwa komórka z martwymi s¹siadami nie mo¿e zmieniæ stanu)
	}

	for (int i = 0; i < height * width; i++) map[i] = map[i] % 2; //usuñ robocze wartoœci komórek (obliczona tablica ma zawierac tylko 1 lub 0)
}

int* Mapa::generateNeighbourNums(int cellNum){
	int row = cellNum / width;
	int col = cellNum % width;

	int above = (row == 0) ? ((zawijanie) ? height - 1 : -1) : row - 1; //rz¹d nad komórk¹, jeœli rz¹d == 0 i mamy zawijaæ to bierzemy najni¿szy rz¹d, jeœli nie mamy zawijaæ to ustawiamy na -1
	int below = (row == height - 1) ? ((zawijanie) ? 0 : -1) : row + 1; //rz¹d pod komórk¹, jeœli rz¹d == height - 1 i mamy zawijaæ to bierzemy rz¹d == 0, jeœli nie mamy zawijaæ to ustawiamy na -1
	int left = (col == 0) ? ((zawijanie) ? width - 1 : -1) : col - 1; //kolumna na lewo od komórki, jeœli kolumna == 0 i mamy zawijaæ to bierzemy kolumna == width - 1, jeœli nie mamy zawijaæ to ustawiamy na -1
	int right = (col == width - 1) ? ((zawijanie) ? 0 : -1) : col + 1; //kolumna na prawo od komórki, jeœli kolumna == width - 1 i mamy zawijaæ to bierzemy kolumna == 0, jeœli nie mamy zawijaæ to ustawiamy na -1

	int* coords = new int[16]; //koordynaty komórek do sprawdzenia
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

int Mapa::decideCellState(int neighbourCnt, int currentState) { //funkcja decyduj¹ca nastêpny stan komórki na podstawie liczby s¹siadów
	if (neighbourCnt < 2) return (currentState == 1) ? 4 : 2;	//ma za ma³o s¹siadów i jest ¿ywa, to ma um¿eæ		(nowa martwa : sprawdzona martwa)
	else if (neighbourCnt > 3) return (currentState == 1) ? 4 : 2;	//ma za du¿o s¹siadów i jest ¿ywa to ma um¿eæ	(nowa martwa : sprawdzona martwa)
	else if (neighbourCnt == 3) return (currentState == 0) ? 5 : 3;	//ma 3 s¹siadów i jest martwa to ma o¿yæ		(nowa ¿ywa : sprawdzona ¿ywa)
	else return (currentState == 0) ? 2 : 3; //stan ma pozostaæ niezmieniony		(sprawdzona martwa : sprawdzona ¿ywa)
}

void Mapa::calculateNeighbours(int cellNum) { //oblicz s¹siadów podanej komórki
	int* coords = generateNeighbourNums(cellNum);

	for (int i = 0; i < 16; i += 2) { //przejdŸ po komórkach do sprawdzenia
		if (coords[i] < 0 || coords[i + 1] < 0) continue; //jeœli któraœ wartoœæ jest ujemna, to znaczy ¿e nie mamy zawijaæ i dana komórka wychodzi poza planszê, nale¿y pomin¹æ
		int num = coords[i] * width + coords[i + 1]; //numer komórki
		if (map[num] > 5) map[num] = map[num] % 2; //usuwamy wartoœci robocze ustawione przez hologram
		if (map[num] != 0) continue; //obliczamy tylko martwe niesprawdzone komórki (¿ywe zostan¹ obliczone przez pêtlê g³ównej funkcji)
		map[num] = decideCellState(countNeighbours(num), map[num]); //oblicz nowy stan komórki
	}
	delete[]coords;
}

int Mapa::countNeighbours(int cellNum) { //policz ¿ywych s¹siadów komórki
	int count = 0;
	int* coords = generateNeighbourNums(cellNum);

	for (int i = 0; i < 16; i += 2) {
		if (coords[i] < 0 || coords[i + 1] < 0) continue; //jeœli któraœ wartoœæ jest ujemna, to znaczy ¿e nie mamy zawijaæ i dana komórka wychodzi poza planszê, nale¿y pomin¹æ
		int num = coords[i] * width + coords[i + 1]; //numer komórki
		if (map[num] > 5) map[num] = map[num] % 2;//usuwamy pomalowane komórki
		if (map[num] == 1 || map[num] == 3 || map[num] == 4) count++; //¿ywa / sprawdzona ¿ywa / nowa martwa (by³a ¿ywa)
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
