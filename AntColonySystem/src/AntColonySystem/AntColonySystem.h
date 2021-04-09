#pragma once
#include "../base.h"

namespace AntColonySystem
{
	struct SimSettings
	{
		int MinX = 0;
		int MaxX = 1500;
		int MinY = 0;
		int MaxY = 800;

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
		float Pheromone = 0.0f;
		float PHeromoneDefault = 0.0f;// caluclated at setup
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

	class AntColonySystem
	{
	public:
		void Setup(const SimSettings& params = SimSettings());
		void Run(int TotalItertions, int TotalNodes, int TotalAnts);

	private:
		void SetupNodesAndEdges(int TotalNodes);
		float GetDenominator(int node);
	private:
		SimSettings Params;
		std::vector<std::pair<int, int>> Nodes;		
		std::map<int, std::map<int, EdgeData>>  Edges; //Adjacency list 	
		std::vector<AntData> Ants;
	};
}
