#pragma once
#include "../base.h"
namespace ACO
{	
	struct NodeSettings
	{
		std::pair<int, int> xAxis;
		std::pair<int, int> yAxis;		
		NodeSettings(int minX = 100, int maxX = 1500, int minY = 100, int maxY = 800) : xAxis({ minX, maxX }), yAxis({ minY, maxY }) { }
	};

	class NodeManager
	{
	public:
		static const std::vector<std::pair<int, int>> SetParamsAndMakeNodes(int node_count, int minX = 100, int maxX = 1500, int minY = 100, int maxY = 800);
		static void SetNodeParams(int minX = 100, int maxX = 1500, int minY = 100, int maxY = 800);
		static void MakeNodes(int node_count);
		
		static const std::vector<std::pair<int, int>> GetNodes();
	private: 
		static NodeSettings _NodeParams;
		static std::vector<std::pair<int, int>> _Nodes;
	};
}