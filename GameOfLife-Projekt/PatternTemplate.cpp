#include "PatternTemplate.h"

PatternTemplate::PatternTemplate(const int patternW, const int patternH, const std::vector<int> patternData, const std::string patternName)
{
	height = patternH;
	width = patternW;
	name = patternName;
	data = patternData;

}