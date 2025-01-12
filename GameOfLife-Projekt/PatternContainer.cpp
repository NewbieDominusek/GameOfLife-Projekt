#include "PatternContainer.h"

PatternContainer::PatternContainer() {

	int gliderSize = 3; //domyœlny glider na wszelki wypadek (¿eby mo¿na by³o coœ wkleiæ)
	std::vector<int> gliderData = {
		0,0,1,
		1,0,1,
		0,1,1
	};

	patterns.push_back(PatternTemplate(gliderSize, gliderSize, gliderData, "Glider")); //dodaj glider do wektora ze wzorkami




	std::fstream plik("patterns.txt", std::fstream::in); //otwórz plik
	std::string tempName, helper; //string na nazwê i helper do konwersji danych
	std::vector<int> tempData; //wektor na tymczasowe dane
	int tempW = 0;	//tymczasowa zmienna na szerokoœæ
	int tempH = 0;	//tymczasowa zmienna na wysokoœæ
	char tempC; //zmienna na znak

	if (plik.is_open()) {
		while (plik.get(tempC)) { //przechodzenie po znakach jeden po jednym


			if (tempC == 'h') { //pêtla z wysokoœci¹
				helper.clear(); //wyczyœæ zmienn¹ pomocnicz¹
				while (plik.get(tempC)) { //przechodŸ po danych z wysokoœci¹
					if (tempC == ':') continue;//ignoruj dwukropek
					if (tempC == '.') break; //jak dojdziesz do kropki to zakoñcz
					helper += tempC; //zbieraj cyfry wysokoœci
				}
				tempH = std::stoi(helper); //konwertuj na liczbê i zapisz
			}



			if (tempC == 'w') { //pêtla z szerokoœci¹
				helper.clear();
				while (plik.get(tempC)) { //przechodŸ po danych z szerokoœci¹
					if (tempC == ':') continue;//ignoruj dwukropek
					if (tempC == '.') break; //jak dojdziesz do kropki to zakoñcz
					helper += tempC; //zbieraj cyfry szerokoœci
				}
				tempW = std::stoi(helper); //konwertuj na liczbê i zapisz
			}



			if (tempC == 'd') { //pêtla z danymi
				tempData.clear();
				while (plik.get(tempC)) { //przechodŸ po danych
					if (tempC == ':') continue; //ignoruj dwukropek
					if (tempC == '.') break; //jak dojdziesz do kropki to zakoñcz
					else if (tempC == '1' || tempC == '0') tempData.push_back((tempC == '0')?0 :1 ); //jeœli jest inny znak ni¿ 1 albo 0 (np \n) to go ignoruj
					//zbieraj dane o komórkach
				}
			}


			if (tempC == 'p') { //pêtla z nazw¹, tutaj te¿ dodajemy wzorek do listy wzorków
				tempName.clear();
				while (plik.get(tempC)) { //przechodŸ po danych
					if (tempC == ':') continue; //ignoruj dwukropek
					if (tempC == '.') break; //jak dojdziesz do kropki to zakoñcz
					tempName += tempC;
				}

				if (tempData.size() == tempH * tempW) {
					patterns.push_back(PatternTemplate(tempW, tempH, tempData, tempName)); //dodajemy tylko jeœli rozmiar siê zgadza!!!
				}
			}
		}
	}
	plik.close();
}