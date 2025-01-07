#include "PatternContainer.h"

PatternContainer::PatternContainer() {

	int gliderSize = 3; //domyœlny glider na wszelki wypadek xd
	std::vector<int> gliderData = {
		0,0,1,
		1,0,1,
		0,1,1
	};

	patterns.push_back(PatternTemplate(gliderSize, gliderSize, gliderData, "Glider"));




	std::fstream plik("patterns.txt", std::fstream::in);
	std::string tempName, helper; //string na nazwê i helper do konwersji danych
	std::vector<int> tempData;
	int tempW = 0, tempH = 0;
	char tempC;

	if (plik.is_open()) {
		while (plik.get(tempC)) { //przechodzenie po znakach jeden po jednym


			if (tempC == 'h') { //pêtla z wysokoœci¹
				plik.get(tempC); //ignoruj dwukropek
				helper.clear();
				while (plik.get(tempC)) { //przechodŸ po danych z wysokoœci¹
					if (tempC != '.') helper += tempC; //i zbieraj cyfry wysokoœci
					else break; //jak dojdziesz to kropki to zakoñcz
				}
				tempH = std::stoi(helper); //konwertuj na liczbê i zapisz
				helper.clear();
			}



			if (tempC == 'w') { //pêtla z szerokoœci¹
				plik.get(tempC); //ignoruj dwukropek
				helper.clear();
				while (plik.get(tempC)) { //przechodŸ po danych z szerokoœci¹
					if (tempC != '.') helper += tempC; //i zbieraj cyfry szerokoœci
					else break; //jak dojdziesz to kropki to zakoñcz
				}
				tempW = std::stoi(helper); //konwertuj na liczbê i zapisz
				helper.clear();
			}



			if (tempC == 'd') { //pêtla z danymi
				plik.get(tempC); //ignoruj dwukropek
				tempData.clear();
				while (plik.get(tempC)) { //przechodŸ po danych
					if (tempC == '.') break; //jak dojdziesz to kropki to zakoñcz
					else if (tempC == '1' || tempC == '0') tempData.push_back((tempC == '0')?0 :1 ); //jeœli jest inny znak ni¿ 1 albo 0 (np \n) to przejdŸ do kolejnej iteracji
					//zbieraj dane o komórkach
				}
			}


			if (tempC == 'p') { //pêtla z nazw¹, tutaj te¿ dodajemy wzorek do listy wzorków
				plik.get(tempC); //ignoruj dwukropek
				tempName.clear();
				while (plik.get(tempC)) { //przechodŸ po danych
					if (tempC != '.') tempName += tempC;
					else break; //jak dojdziesz to kropki to zakoñcz
				}

				if (tempData.size() == tempH * tempW) {
					patterns.push_back(PatternTemplate(tempW, tempH, tempData, tempName)); //dodajemy tylko jeœli rozmiar siê zgadza!!!
				}
			}
		}
	}
	plik.close();
}