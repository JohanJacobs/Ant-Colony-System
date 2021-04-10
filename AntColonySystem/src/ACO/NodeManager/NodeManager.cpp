#include "NodeManager.h"

namespace ACO
{
	ACO::NodeSettings ACO::NodeManager::_NodeParams;
	std::vector<std::pair<int,int>> ACO::NodeManager::_Nodes;

	void NodeManager::SetNodeParams(int minX, int maxX, int minY, int maxY)
	{
		_NodeParams = NodeSettings(minX, maxX, minY, maxY);
	}

	void NodeManager::MakeNodes(int node_count)
	{
		_Nodes.clear();
		for (int i = 0; i < node_count; i++)
		{
			int x = Random::get<int>(_NodeParams.xAxis.first, _NodeParams.xAxis.second);
			int y = Random::get<int>(_NodeParams.yAxis.first, _NodeParams.yAxis.second);
			_Nodes.emplace_back(std::make_pair(x, y));
		}
	}

	const std::vector<std::pair<int, int>> NodeManager::SetParamsAndMakeNodes(int node_count, int minX, int maxX, int minY, int maxY)
	{
		SetNodeParams(minX, maxX, minY, maxY);
		MakeNodes(node_count);
		return GetNodes();
	}

	const std::vector<std::pair<int, int>> NodeManager::GetNodes()
	{
		if (_Nodes.size() == 0)
		{
			std::cerr << "Nodes was not created! run ACO::NodeSettings::MakeNodes(x)" << std::endl;		
			exit(1);
		}
		return _Nodes;
	}
}