#pragma once
#include "PatternTemplate.h"
#include <vector>
#include <fstream>
#include <string>

class PatternContainer
{
public:
	std::vector<PatternTemplate> patterns; //wektor ze wzorkami
	PatternContainer(); //konstruktor z ³adowanie wzorków z pliku
};