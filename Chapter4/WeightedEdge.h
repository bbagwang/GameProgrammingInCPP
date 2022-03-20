#pragma once

#include <vector>

struct WeightedGraphNode
{
	std::vector<WeightedEdge*> Edges;
};

struct WeightedEdge
{
	//이 엣지에 어떤 노드가 연결되어 있는가?
	WeightedGraphNode* From;
	WeightedGraphNode* To;
	
	//이 엣지의 가중치
	float Weight;
};