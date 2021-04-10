# Ant-Colony-System
Base on the paper M. Dorigo and L. M. Gambardella, "Ant colony system: a cooperative learning approach to the traveling salesman problem," in IEEE Transactions on Evolutionary Computation, vol. 1, no. 1, pp. 53-66, April 1997, doi: 10.1109/4235.585892.

# Motivation
Understand the algorithm: Code was writted to illustrate / understand the algorithms steps rather than be efficient code.

# Usage 
#include "ACO/ACO.h"

To create nodes:__
    ACO::NodeManager::SetParamsAndMakeNodes(total_nodes, minX, maxX, minY, maxY); 

To run a simluation with the created nodes :__
    ACO::AntSystem::SimSettings ant_system_default_settings;__
    ACO::AntSystem::AntSystem ant_system(ant_system_default_settings);__
    float as_best = ant_system.Run(ACO::NodeManager::GetNodes(), iterations, total_ants);__

    ACO::AntColonySystem::SimSettings ant_colony_system_default_settings;__
    ACO::AntColonySystem::AntColonySystem ant_colony_system(ant_colony_system_default_settings);__
    float acs_best = ant_colony_system.Run(ACO::NodeManager::GetNodes(), iterations, total_ants);__

# Class: AntSystem: 
The original algorithm discussed in the background section of the paper. Based on M. Dorigo, Optimization learning and natural algorithms, 1992.

# Class: AntColonySystem
The propsed algorithm in this paper.

# Required Libraries:
 - Used Effolkronium for random numbers: https://github.com/effolkronium/random
 - if a custom random function is used updated the base.h and relevant "random::get<>" in codebase.

# General Nodes
 - Currently uses Randomly generated Node locations.
 - All nodes are fully connected.
 - Ant start positions are random.
