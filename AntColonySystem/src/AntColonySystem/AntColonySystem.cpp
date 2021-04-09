#include "AntColonySystem.h"

/* TODO: Update to nearest neighbor heuristic! */

namespace AntColonySystem
{

	void AntColonySystem::Setup(const SimSettings& params)
	{
		Params = params;
	}
	void AntColonySystem::Run(int TotalItertions, int TotalNodes, int TotalAnts)
	{
		/* setup nodes and Edges  */
		SetupNodesAndEdges(TotalNodes);

		/* Run sim */
		int iteration = 0;
		while (iteration < TotalItertions)
		{
			/* setup ants */
			Ants.clear();
			for (int AntCounter = 0; AntCounter < TotalAnts; AntCounter++)
			{
				int start_node = Random::get<int>(0, TotalNodes - 1);
				auto& ant = Ants.emplace_back(AntData(start_node));
				ant.NodesVisited.push_back(start_node);
			}

			/* 
			   each ant apply state transition rule to incrementally build a solution 
			   and a local pheromone updating rule Formulas (3) and (1)
			*/
			for (auto& ant : Ants)
			{
				do /* build ant tour */
				{
					/* get the denominator for formulas*/
					float denominator = 0;
					float total = 0;
					for (auto [node_number, edge] : Edges[ant.CurrentNode])
					{
						float pheromone = std::pow(edge.Pheromone * edge.InvDist, Params.beta);
						denominator += pheromone;
					}

					/* determine state transition rule and get the next node*/
					float q = Random::get<float>(0, 1);
					bool AntShouldExploit = (q <= Params.q0);
					if (AntShouldExploit)
					{
						// take the best path
						int best_edge_node = 0;
						float best_edge_score = std::numeric_limits<float>::max();
						for (auto [node_number, edge] : Edges[ant.CurrentNode])
						{
							float edge_score = std::pow(edge.Pheromone * edge.InvDist, 2.0f) / denominator;
							if (edge_score > best_edge_score)
							{
								best_edge_node = node_number;
								best_edge_score = edge_score;
							}
						}
						ant.CurrentNode = best_edge_node;
						ant.NodesVisited.push_back(best_edge_node);
					}
					else
					{
						// explore as in AntSystem
						int move_to_node = -1;						
						float sum_probability= 0.0f;

						for (auto [node_number, edge] : Edges[ant.CurrentNode])
						{
							bool already_visisted = std::find_if(ant.NodesVisited.begin(), ant.NodesVisited.end(),
								[node_number](int a)
								{
									return a == node_number;
								}) != ant.NodesVisited.end();
							if (already_visisted)
								continue;

							/* test if ant can move */
							float edge_probability = std::pow(edge.Pheromone * edge.InvDist, 2.0f) / denominator;
							sum_probability += edge_probability;
							float random_value = Random::get<float>(0.0f, 1.0f);

							bool make_move = (sum_probability >= random_value);
							if (make_move)
							{
								move_to_node = node_number;								
								break;
							}
						}
						
						/* move the ant */						
						ant.CurrentNode = move_to_node;
						ant.NodesVisited.push_back(move_to_node);
						if (ant.NodesVisited.size() == Nodes.size())
						{
							ant.Alive = false;
							ant.GoodAnt = true;
						}
					}
				} while (ant.NodesVisited.size() <= Nodes.size() && ant.Alive);
			}

			/* Local Pheromone updating according to (5)*/			
			for (auto ant : Ants)
			{
				bool this_ant_was_bad = !ant.GoodAnt;
				if (this_ant_was_bad)
					continue;

				for (size_t node_index = 1; node_index < ant.NodesVisited.size(); node_index + 1)
				{
					size_t current_node = node_index - 1;
					size_t next_node = node_index;

					/* formula (5) */
					//float antQ = Params.gamma*max(z) // see page 4
					//float edge_pheromone = Edges[current_node][next_node].Pheromone + Params.p*Delta(r,s)
					float new_pheromone = (1 - Params.p) * 1;
				}


			}
			/*heuristic based on 3-opt*/

			/* global pheromone updating rule is applied */
			iteration += 1;
		}
	}

	void AntColonySystem::SetupNodesAndEdges(int TotalNodes)
	{
		/* reset everything */
		Nodes.clear();
		Edges.clear();

		/* setup random nodes */
		std::cout << "Creating " << TotalNodes << " nodes.\n";
		for (int i = 0; i < TotalNodes; i++)
		{
			int x = Random::get<int>(Params.MinX, Params.MaxX);
			int y = Random::get<int>(Params.MinY, Params.MaxY);
			auto new_node = Nodes.emplace_back(std::make_pair(x, y));
		}

		/* setup all the edges, fully connected, Undirected graph*/
		std::cout << "Creating " << TotalNodes * (TotalNodes - 1) << " edges.\n";
		for (int i = 0; i < Nodes.size(); i++)
		{
			for (int j = 0; j < Nodes.size(); j++)
			{
				// skip if they are the same 
				if (i == j)
					continue;
				Edges[i][j].Distance = Dist(Nodes[i], Nodes[j]);
				Edges[i][j].InvDist = 1 / Edges[i][j].Distance;				
			}
		}
		/* use nearest neighbor for the pheromone initial value */

		for (auto edge : Edges)
		{

		}
	}

	/* the denominator returns the "sum product" of the available edges to the ant's current node */
	float AntColonySystem::GetDenominator(int current_node)
	{
		return 0;
	}	
}