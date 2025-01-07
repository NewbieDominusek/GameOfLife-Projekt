#pragma once
#include "PatternTemplate.h"
#include <vector>
#include <fstream>
#include <string>

class PatternContainer
{
public:
	std::vector<PatternTemplate> patterns;
	PatternContainer();
};