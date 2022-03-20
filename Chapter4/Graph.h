#pragma once

#include <vector>

struct GraphNode
{
	//각 노드는 인접 노드의 포인터들을 가지고 있다.
	std::vector<GraphNode*> Adjacent;
};

struct Graph
{
	//그래프는 노드들을 포함한다.
	std::vector<GraphNode*> Nodes;
};
