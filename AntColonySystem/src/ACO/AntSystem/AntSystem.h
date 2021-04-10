#pragma once
#include "../base.h"

namespace ACO::AntSystem
{
	struct SimSettings
	{
		float Beta = 2.0f;

		float PheromoneMin = 0.01f; // also used as the default 
		float PheromoneDecay = 0.1f;	

		SimSettings()=default;
	};

	struct NodeSettings
	{
		int MinX = 100;
		int MinY = 100;
		int MaxX = 800;
		int MaxY = 1500;
	};

	struct EdgeData
	{
		float Distance = 0.0f;
		float Pheromone = 0.0f;
		float InvDist = 0.0f;		
		EdgeData() {}
		EdgeData(const EdgeData& other) :Distance(other.Distance), Pheromone(other.Pheromone), InvDist(other.InvDist) {};
	};

	struct AntData
	{
		int CurrentNode;
		bool Alive = true;
		bool GoodAnt = true;
		std::vector<int> NodesVisited;
		float DistanceTraveled = 0;
		AntData(int start_node) :CurrentNode(start_node) {};

	};

	class AntSystem
	{
	public:
		AntSystem(SimSettings settings = SimSettings());
		~AntSystem()=default;		
		float Run(std::vector < std::pair<int, int>> SimNodes, int TotalItertions, int TotalAnts);

	private:
		void SetupEdges();
	private:
		NodeSettings NodeParams;
		SimSettings Params;
		std::vector<std::pair<int, int>> Nodes;
		std::map<int, std::map<int, EdgeData>>  Edges; //Adjacency list 	
		std::vector<AntData> Ants;
	};
}