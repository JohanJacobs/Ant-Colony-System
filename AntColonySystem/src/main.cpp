
#include "ACO/ACO.h"

int main(int argc, char *ergv[])
{
	int total_nodes = 500;
	int total_ants = 10;
	int iterations = 50;

	ACO::NodeManager::SetParamsAndMakeNodes(total_nodes, 100, 1500, 100, 800);
		
	ACO::AntSystem::SimSettings ant_system_defaults;
	ACO::AntSystem::AntSystem ant_system(ant_system_defaults);	
	float as_best = ant_system.Run(ACO::NodeManager::GetNodes(),iterations, total_ants);


	ACO::AntColonySystem::SimSettings ant_colony_system_defaults;
	ACO::AntColonySystem::AntColonySystem ant_colony_system(ant_colony_system_defaults);
	float acs_best = ant_colony_system.Run(ACO::NodeManager::GetNodes(), iterations, total_ants);
	std::cout << "-- Results -- \n" 
				 "AntSystem: " << as_best << "\n"
		         "AntColonySystem: " << acs_best;
	return 0;
}