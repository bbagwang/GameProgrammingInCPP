#pragma once

#include <vector>

struct WeightedGraphNode
{
	std::vector<WeightedEdge*> Edges;
};

struct WeightedEdge
{
	//�� ������ � ��尡 ����Ǿ� �ִ°�?
	WeightedGraphNode* From;
	WeightedGraphNode* To;
	
	//�� ������ ����ġ
	float Weight;
};