#pragma once
#include "../base.h"

namespace ACO::AntColonySystem
{
	struct SimSettings
	{
 		float beta = 2.0f;
		float gamma = 0.7f;
		float q0 = 0.9f; // determines the relative importance of exploitation versus exploration 
		float p = 0.1f;

		float PheromoneMin = 0.01f; // also used as the default 
		float PheromoneDecay = 0.1f;		
		SimSettings() = default;
	};

	struct EdgeData
	{
		float Distance = 0.0f;
		float InvDist = 0.0f;
		float Pheromone = 0.0f;
		float PheromoneDefault = 0.0f;// calculated at setup
		EdgeData() {}
		EdgeData(const EdgeData& other) :Distance(other.Distance), Pheromone(other.Pheromone), InvDist(other.InvDist) {};
	};

	struct AntData
	{
		int CurrentNode;
		bool Alive = true;
		bool GoodAnt = true;
		std::vector<int> NodesVisited;
		float DistanceTraveled = 0.0f;
		AntData(int start_node) :CurrentNode(start_node) {};

	};

	class AntColonySystem
	{
	public:
		AntColonySystem(SimSettings settings = SimSettings());	
		~AntColonySystem()=default;
		float Run(std::vector<std::pair<int, int>> SimNodes, int TotalItertions, int TotalAnts);

	private:
		void SetupEdges();		
	private:
		SimSettings Params;
		std::vector<std::pair<int, int>> Nodes;		
		std::map<int, std::map<int, EdgeData>>  Edges; //Adjacency list 	
		std::vector<AntData> Ants;
	};
}
