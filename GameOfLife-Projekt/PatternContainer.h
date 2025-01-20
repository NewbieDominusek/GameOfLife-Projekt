#pragma once
#include "PatternTemplate.h"
#include <vector>
#include <fstream>
#include <string>

class PatternContainer
{
public:
	int wybranyWzorek = 0;
	std::vector<PatternTemplate> patterns; //wektor ze wzorkami
	PatternContainer(); //konstruktor z ³adowanie wzorków z pliku
};