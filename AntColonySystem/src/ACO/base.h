#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <iostream>

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

static void PrintRoute( float distance,bool print_route, const std::vector <int>& route)
{
	std::cout <<"  Shortest Distance: " << distance << "\n";

	if (print_route)
	{
		std::cout << "  Route information: ";
		for (const auto& n : route)
			std::cout << n << " - ";
		std::cout << "\n";
	}
}

static bool AntAlreadyVisistedNode(int node_number, const std::vector<int>& ant_path)
{
	auto result = std::find_if(ant_path.begin(), ant_path.end(),
		[node_number](int a)
		{
			return a == node_number;
		});
		
	return !(result == ant_path.end());
}
