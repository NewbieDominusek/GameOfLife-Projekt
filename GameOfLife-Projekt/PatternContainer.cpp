#include "PatternContainer.h"

PatternContainer::PatternContainer() {

	int gliderSize = 3; //domy�lny glider na wszelki wypadek (�eby mo�na by�o co� wklei�)
	std::vector<int> gliderData = {
		0,0,1,
		1,0,1,
		0,1,1
	};

	patterns.push_back(PatternTemplate(gliderSize, gliderSize, gliderData, "Glider")); //dodaj glider do wektora ze wzorkami




	std::fstream plik("patterns.txt", std::fstream::in); //otw�rz plik
	std::string tempName, helper; //string na nazw� i helper do konwersji danych
	std::vector<int> tempData; //wektor na tymczasowe dane
	int tempW = 0;	//tymczasowa zmienna na szeroko��
	int tempH = 0;	//tymczasowa zmienna na wysoko��
	char tempC; //zmienna na znak

	if (plik.is_open()) {
		while (plik.get(tempC)) { //przechodzenie po znakach jeden po jednym


			if (tempC == 'h') { //p�tla z wysoko�ci�
				helper.clear(); //wyczy�� zmienn� pomocnicz�
				while (plik.get(tempC)) { //przechod� po danych z wysoko�ci�
					if (tempC == ':') continue;//ignoruj dwukropek
					if (tempC == '.') break; //jak dojdziesz do kropki to zako�cz
					helper += tempC; //zbieraj cyfry wysoko�ci
				}
				tempH = std::stoi(helper); //konwertuj na liczb� i zapisz
			}



			if (tempC == 'w') { //p�tla z szeroko�ci�
				helper.clear();
				while (plik.get(tempC)) { //przechod� po danych z szeroko�ci�
					if (tempC == ':') continue;//ignoruj dwukropek
					if (tempC == '.') break; //jak dojdziesz do kropki to zako�cz
					helper += tempC; //zbieraj cyfry szeroko�ci
				}
				tempW = std::stoi(helper); //konwertuj na liczb� i zapisz
			}



			if (tempC == 'd') { //p�tla z danymi
				tempData.clear();
				while (plik.get(tempC)) { //przechod� po danych
					if (tempC == ':') continue; //ignoruj dwukropek
					if (tempC == '.') break; //jak dojdziesz do kropki to zako�cz
					else if (tempC == '1' || tempC == '0') tempData.push_back((tempC == '0')?0 :1 ); //je�li jest inny znak ni� 1 albo 0 (np \n) to go ignoruj
					//zbieraj dane o kom�rkach
				}
			}


			if (tempC == 'p') { //p�tla z nazw�, tutaj te� dodajemy wzorek do listy wzork�w
				tempName.clear();
				while (plik.get(tempC)) { //przechod� po danych
					if (tempC == ':') continue; //ignoruj dwukropek
					if (tempC == '.') break; //jak dojdziesz do kropki to zako�cz
					tempName += tempC;
				}

				if (tempData.size() == tempH * tempW) {
					patterns.push_back(PatternTemplate(tempW, tempH, tempData, tempName)); //dodajemy tylko je�li rozmiar si� zgadza!!!
				}
			}
		}
	}
	plik.close();
}