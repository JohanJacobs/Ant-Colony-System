#pragma once
#include "../base.h"

namespace AntSystem
{
	struct SimSettings
	{
		int TotalNodes = 0;
		int TotalAnts = 0;
		int Iterations = 0;

		float Beta = 2.0f;

		float PheromoneMin = 0.01f; // also used as the default 
		float PheromoneDecay = 0.1f;		
		SimSettings(int iterations = 10,int NodeCount = 10, int AntCount = 10) : Iterations(iterations), TotalNodes(NodeCount), TotalAnts(AntCount) 
		{	};
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
		AntSystem();
		~AntSystem();
		void Setup(const SimSettings& params = SimSettings());
		void Run();

	private:

	private:
		NodeSettings NodeParams;
		SimSettings Params;
		std::vector<std::pair<int, int>> Nodes;
		std::map<int, std::map<int, EdgeData>>  Edges; //Adjacency list 	
		std::vector<AntData> Ants;
	};
}