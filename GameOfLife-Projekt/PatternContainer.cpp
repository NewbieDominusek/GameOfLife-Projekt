#include "PatternContainer.h"

PatternContainer::PatternContainer() {

	int gliderSize = 3; //domy�lny glider na wszelki wypadek xd
	std::vector<int> gliderData = {
		0,0,1,
		1,0,1,
		0,1,1
	};

	patterns.push_back(PatternTemplate(gliderSize, gliderSize, gliderData, "Glider"));




	std::fstream plik("patterns.txt", std::fstream::in);
	std::string tempName, helper; //string na nazw� i helper do konwersji danych
	std::vector<int> tempData;
	int tempW = 0, tempH = 0;
	char tempC;

	if (plik.is_open()) {
		while (plik.get(tempC)) { //przechodzenie po znakach jeden po jednym


			if (tempC == 'h') { //p�tla z wysoko�ci�
				plik.get(tempC); //ignoruj dwukropek
				helper.clear();
				while (plik.get(tempC)) { //przechod� po danych z wysoko�ci�
					if (tempC != '.') helper += tempC; //i zbieraj cyfry wysoko�ci
					else break; //jak dojdziesz to kropki to zako�cz
				}
				tempH = std::stoi(helper); //konwertuj na liczb� i zapisz
				helper.clear();
			}



			if (tempC == 'w') { //p�tla z szeroko�ci�
				plik.get(tempC); //ignoruj dwukropek
				helper.clear();
				while (plik.get(tempC)) { //przechod� po danych z szeroko�ci�
					if (tempC != '.') helper += tempC; //i zbieraj cyfry szeroko�ci
					else break; //jak dojdziesz to kropki to zako�cz
				}
				tempW = std::stoi(helper); //konwertuj na liczb� i zapisz
				helper.clear();
			}



			if (tempC == 'd') { //p�tla z danymi
				plik.get(tempC); //ignoruj dwukropek
				tempData.clear();
				while (plik.get(tempC)) { //przechod� po danych
					if (tempC == '.') break; //jak dojdziesz to kropki to zako�cz
					else if (tempC == '1' || tempC == '0') tempData.push_back((tempC == '0')?0 :1 ); //je�li jest inny znak ni� 1 albo 0 (np \n) to przejd� do kolejnej iteracji
					//zbieraj dane o kom�rkach
				}
			}


			if (tempC == 'p') { //p�tla z nazw�, tutaj te� dodajemy wzorek do listy wzork�w
				plik.get(tempC); //ignoruj dwukropek
				tempName.clear();
				while (plik.get(tempC)) { //przechod� po danych
					if (tempC != '.') tempName += tempC;
					else break; //jak dojdziesz to kropki to zako�cz
				}

				if (tempData.size() == tempH * tempW) {
					patterns.push_back(PatternTemplate(tempW, tempH, tempData, tempName)); //dodajemy tylko je�li rozmiar si� zgadza!!!
				}
			}
		}
	}
	plik.close();
}