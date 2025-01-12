#pragma once
#include <string>
#include <vector>

class PatternTemplate
{
public:
	int height; //wysokoœæ wzorku
	int width; //szerokoœæ wzorku
	std::vector<int> data; //dane wzorku
	std::string name; //nazwa
	PatternTemplate(int patternW, int patternH, std::vector<int> patternData, std::string patternName); //konstruktor
};