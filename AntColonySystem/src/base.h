#pragma once

#include <iostream>
#include <map>
#include <vector>

/* for the random numbers in Effolkronium */
#include "Effolkronium/random.hpp"
using Random = effolkronium::random_static;

inline float SqDist(const std::pair<int, int>& a, const std::pair<int, int>& b) 
{ 
	return std::pow(float(a.first - b.first), 2.0f) + std::pow(float(a.second - b.second), 2.0f); 
}

inline float Dist(const std::pair<int, int>& a, const std::pair<int, int>& b)
{ 
	return std::sqrt(SqDist(a, b)); 
}
