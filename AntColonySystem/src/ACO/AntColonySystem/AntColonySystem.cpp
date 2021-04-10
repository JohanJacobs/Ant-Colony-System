#include "AntColonySystem.h"

/* TODO: Update to nearest neighbor heuristic! */

namespace ACO::AntColonySystem
{
	AntColonySystem::AntColonySystem(SimSettings settings /*= SimSettings()*/)
	{
		Params = settings;
		std::cout << "\n\nTSP with Ant Colony System\n";
	}

	float AntColonySystem::Run(std::vector < std::pair<int, int>> SimNodes, int TotalItertions, int TotalAnts)
	{
		Nodes = SimNodes;
		/* setup nodes and Edges  */
		SetupEdges();

		/* keep track of the best ant */
		float global_best_ant_distance = std::numeric_limits<float>::max();
		std::vector<int> global_best_ant_path;
		bool global_best_ant_changed = false;
			

		for (int iteration = 0; iteration < TotalItertions; iteration++)
		{
			std::cout << "Iteration[" << iteration << "]\n";

			/* setup ants */
			Ants.clear();
			for (int ant_counter = 0; ant_counter < TotalAnts; ant_counter++)
			{
				int start_node = Random::get<int>(0, int(Nodes.size()) - 1);
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
						/* check if the ant visited this node before */						
						if (AntAlreadyVisistedNode(node_number, ant.NodesVisited))
							continue;

						float pheromone = std::pow(edge.Pheromone * edge.InvDist, Params.beta);
						denominator += pheromone;
					}

					/* determine state transition rule and get the next node*/
					float q = Random::get<float>(0, 1);
					bool AntShouldExploit = (q <= Params.q0);
					if (AntShouldExploit)
					{
						// take the best path
						int best_edge_node = -1;
						float best_edge_score = -1;
						for (auto [node_number, edge] : Edges[ant.CurrentNode])
						{
							/* check if the ant visited this node before */							
							if (AntAlreadyVisistedNode(node_number, ant.NodesVisited))
								continue;

							/* check if this edge is closer */
							float edge_score = std::pow(edge.Pheromone * edge.InvDist, 2.0f) / denominator;
							bool this_edge_has_better_score = edge_score > best_edge_score;
							if (this_edge_has_better_score)
							{
								best_edge_node = node_number;
								best_edge_score = edge_score;
							}
						}
						ant.DistanceTraveled += Edges[ant.CurrentNode][best_edge_node].Distance;
						ant.NodesVisited.push_back(best_edge_node);
						ant.CurrentNode = best_edge_node;
					}
					else
					{
						// explore as in AntSystem
						int move_to_node = -1;						
						float sum_probability= 0.0f;

						for (auto [node_number, edge] : Edges[ant.CurrentNode])
						{
							if (AntAlreadyVisistedNode(node_number, ant.NodesVisited))
								continue;

							/* test if ant can move */
							float edge_probability = std::pow(edge.Pheromone * edge.InvDist, 2.0f) / denominator;
							sum_probability += edge_probability;
							float random_value = Random::get<float>(0.0f, 1.0f);

							bool ant_should_make_move = (sum_probability >= random_value);
							if (ant_should_make_move)
							{
								move_to_node = node_number;								
								break;
							}
						}
						
						/* move the ant */						
						ant.DistanceTraveled += Edges[ant.CurrentNode][move_to_node].Distance;
						ant.NodesVisited.push_back(move_to_node);
						ant.CurrentNode = move_to_node;

						bool this_ant_visisted_all_nodes = ant.NodesVisited.size() == Nodes.size();
						if (this_ant_visisted_all_nodes)
						{
							ant.Alive = false;
							ant.GoodAnt = true;
						}
					}
				} while (ant.NodesVisited.size() < Nodes.size() && ant.Alive);
			}

			/* Local Pheromone updating according to (5) */			
			/* Here the pheromone is reduced for each edge that an ant traversed */			
			for (auto ant : Ants)
			{
				bool this_ant_was_bad = !ant.GoodAnt;
				if (this_ant_was_bad)
					continue;

				for (size_t node_index = 1; node_index < ant.NodesVisited.size(); node_index ++)
				{
					int current_node = int(node_index) - 1;
					int next_node = int(node_index);
					auto& edge_data = Edges[current_node][next_node]; // reference to the edge data for editing

					/* formula (5) */
					float decay = (1 - Params.p) * edge_data.Pheromone;
					float adjusted_default = Params.p * edge_data.PheromoneDefault;
					float new_pheromone = decay + adjusted_default;
				}
			}
			
			/* calculate the best ant index */						
			for (int current_ant = 1; current_ant < Ants.size(); current_ant++)
			{
				float current_ant_distance = Ants[current_ant].DistanceTraveled;
				bool current_ant_is_better = (current_ant_distance < global_best_ant_distance);

				if (current_ant_is_better)
				{
					global_best_ant_distance = current_ant_distance;
					global_best_ant_path = Ants[current_ant].NodesVisited;
					global_best_ant_changed = true;
				}
			}

			/* Global updating rule, update all the edges with formula (4) */	
			float inv_best_ant_distance = 1 / global_best_ant_distance;						
			for (auto& [from_node, from_node_edges] : Edges)
			{
				for (auto& [too_node, edge_data] : from_node_edges)
				{
					/* was this edge visited by the best ant */
					bool best_ant_visited_this_edge = false;
					for (int index = 1; index < global_best_ant_path.size(); index++)
					{												
						int from = global_best_ant_path[size_t(index) - 1];
						int too = global_best_ant_path[index];
						if (from == from_node  && too == too_node)
						{
							best_ant_visited_this_edge = true;
							break;
						}
					}

					/* update pheromone */
					float decay = (1 - Params.PheromoneDecay) * edge_data.Pheromone;
					float boost = 0;
					
					if (best_ant_visited_this_edge)
						boost = Params.PheromoneDecay * inv_best_ant_distance;

					edge_data.Pheromone = decay + boost;
				}
			}
						
			/* print best routes */			
			if (global_best_ant_changed)
			{
				PrintRoute(global_best_ant_distance, false, global_best_ant_path);
				global_best_ant_changed = false;
			}
			iteration += 1;
		}

		PrintRoute( global_best_ant_distance, true, global_best_ant_path);
		return global_best_ant_distance;
	}

	void AntColonySystem::SetupEdges()
	{
		/* reset everything */		
		Edges.clear();
				
		/* setup all the edges, fully connected, Undirected graph*/		
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
		float nearest_neighbour_distance = 0;

		int current_node = 0;
		float distance = 0;
		
		std::vector <int> visited_nodes;
		visited_nodes.push_back(current_node);	

		while (visited_nodes.size() < Nodes.size())
		{
			int closest_edge = -1;
			float closest_edge_dist = std::numeric_limits<float>::max();
			
			for (auto& edge : Edges[current_node])
			{
				auto [node_number, edge_data] = edge;			
				
				/* check if we visited this node already?*/
				if (AntAlreadyVisistedNode(node_number, visited_nodes))
					continue;

				/* if this node is closer update the data */
				if (edge_data.Distance < closest_edge_dist)
				{
					closest_edge = node_number;
					closest_edge_dist = edge_data.Distance;					
				}
			}
			/* move to closets node */
			nearest_neighbour_distance += closest_edge_dist;
			visited_nodes.push_back(closest_edge);			
		}
		std::cout << "\n";

		float default_pheromone = 1 / (Nodes.size() * nearest_neighbour_distance);

		/* update all edges with default pheromone */
		for (auto& [from_node, from_node_edges] : Edges)
		{
			for (auto& [too_node, edge_data] : from_node_edges)
			{
				edge_data.PheromoneDefault = default_pheromone;
				edge_data.Pheromone = default_pheromone;
			}
		}
	}		
}