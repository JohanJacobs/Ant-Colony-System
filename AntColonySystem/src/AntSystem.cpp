#include "AntSystem.h"

namespace AntSystem
{

	AntSystem::AntSystem()
	{
		std::cout << "TSP with AntSystem\n";
	}

	AntSystem::~AntSystem()
	{

	}

	void AntSystem::Setup(const SimSettings& params)
	{
		Params = params;		

		Nodes.clear();
		Edges.clear();

		/* setup random nodes */
		std::cout << "Creating " << Params.TotalNodes << " nodes.\n";		
		for (int i = 0; i < Params.TotalNodes; i++)
		{
			int x = Random::get<int>(NodeParams.MinX, NodeParams.MaxX);
			int y = Random::get<int>(NodeParams.MinY, NodeParams.MaxY);
			auto new_node = Nodes.emplace_back(std::make_pair(x, y));
		}

		/* setup all the edges, fully connected, Undirected graph*/
		std::cout << "Creating " << Params.TotalNodes * (Params.TotalNodes - 1) << " edges.\n";		
		for (int i = 0; i < Nodes.size(); i++)
		{
			for (int j = 0; j < Nodes.size(); j++)
			{
				// skip if they are the same 
				if (i == j)
					continue;
				Edges[i][j].Distance = Dist(Nodes[i], Nodes[j]);
				Edges[i][j].InvDist = 1 / Edges[i][j].Distance;
				Edges[i][j].Pheromone = Params.PheromoneMin;
			}
		}
	}

	void AntSystem::Run()
	{	

		/* run the simulation */
		std::vector<int> BestRoute;
		float BestRouteDistance = std::numeric_limits<float>::max(); // maximum value for a float.
		int BestRouteIteration = -1;

		for (int iteration = 0; iteration < Params.Iterations; iteration++)
		{
			std::cout << "Iteration [ " << iteration << " ]\n";

			/* setup the ant defaults */
			Ants.clear();
			for (int i = 0; i < Params.TotalAnts; i++)
			{
				int start_node = Random::get<int>(0, Params.TotalNodes - 1);
				auto& ant = Ants.emplace_back(AntData(start_node));
				ant.NodesVisited.push_back(start_node);
			}

			/* move the ants */
			for (auto& ant : Ants)
			{
				do {
					/* Calculate denominator of state transition rule */
					auto denominator = 0.0f;
					for (auto edge : Edges[ant.CurrentNode])
					{
						auto [node_number, edge_data] = edge;
						denominator += std::pow(edge_data.Pheromone * edge_data.InvDist, Params.Beta);;
					}

					/* decide on a direction to move to */
					float highest_probability = -1.0f;
					int MoveToNode = -1;
					for (auto edge : Edges[ant.CurrentNode])
					{
						auto [node_number, edge_data] = edge;

						/* check if we visited this node already?*/
						bool already_visisted = std::find_if(ant.NodesVisited.begin(), ant.NodesVisited.end(),
							[node_number](int a)
							{
								return a == node_number;
							}) != ant.NodesVisited.end();

							if (already_visisted)
								continue;

							/* calculate probability to move */
							float numerator = std::pow(edge_data.Pheromone * edge_data.InvDist, Params.Beta);
							float p = numerator / denominator;

							/* test if ant can move */
							bool MakeMove = (p >= Random::get<float>(0.0f, 1.0f));
							if (MakeMove)
							{
								MoveToNode = node_number;
								break;
							}

							/* save if it is a higher prob for the default move. */
							auto save_higher_probability = p > highest_probability;
							if (save_higher_probability)
							{
								highest_probability = p;
								MoveToNode = node_number;
							}
					}

					if (MoveToNode < 0)
					{
						ant.Alive = false; // no node available to move to 
						ant.GoodAnt = false;
						std::cout << "[ERROR ]  Ant killed! \n";
						break;
					}

					/* move ant to new node */
					ant.DistanceTraveled += Edges[ant.CurrentNode][MoveToNode].Distance;
					ant.CurrentNode = MoveToNode;
					ant.NodesVisited.push_back(MoveToNode);

					/* check for terminating condition */
					auto VisistedAllNodes = ant.NodesVisited.size() == Nodes.size();
					if (VisistedAllNodes)
						ant.Alive = false;

				} while (ant.Alive);
			}

			/* update pheromone on edges */
			/* decay */
			for (int i = 0; i < Nodes.size(); i++)
			{
				for (int j = 0; j < Nodes.size(); j++)
				{
					if (i == j) // no path to the same node
						continue;

					float decayed_pheromone = (1 - Params.PheromoneDecay) * Edges[i][j].Pheromone;
					Edges[i][j].Pheromone = decayed_pheromone;
				}
			}

			/* deposit pheromone  */
			for (auto ant : Ants)
			{
				if (!ant.GoodAnt)
					continue;

				for (int i = 1; i < ant.NodesVisited.size(); i++)
				{
					auto from_node_index = size_t(i) - 1;
					auto too_node_index = size_t(i);

					auto from_node = ant.NodesVisited[from_node_index];
					auto too_node = ant.NodesVisited[too_node_index];

					auto new_pheromone = 1 / ant.DistanceTraveled;
					Edges[from_node][too_node].Pheromone += new_pheromone;
				}
			}

			for (auto ant : Ants)
			{
				bool BetterRoute = BestRouteDistance > ant.DistanceTraveled;
				if (BetterRoute)
				{
					BestRouteDistance = ant.DistanceTraveled;
					BestRouteIteration = iteration;
					BestRoute = ant.NodesVisited;
					std::cout << "    Better Route detected with distance " << BestRouteDistance << "\n";
					std::cout << "    route: ";
					for (auto n : BestRoute)
						std::cout << n << " -> ";
					std::cout << "\n";
				}
			}
		}

		/* print best routes */
		std::cout << " --= RESULTS =-\n"
			"Shortest Distance: " << BestRouteDistance << "\n"
			"Route information:";
		for (auto n : BestRoute)
			std::cout << n << " -> ";
		std::cout << "\n";
	}

}