#pragma once
#include <string>
#include <vector>

class PatternTemplate
{
public:
	int height;
	int width;
	std::vector<int> data;
	std::string name;
	PatternTemplate(int patternW, int patternH, std::vector<int> patternData, std::string patternName);
};