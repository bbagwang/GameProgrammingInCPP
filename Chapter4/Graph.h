#pragma once

#include <vector>

struct GraphNode
{
	//�� ���� ���� ����� �����͵��� ������ �ִ�.
	std::vector<GraphNode*> Adjacent;
};

struct Graph
{
	//�׷����� ������ �����Ѵ�.
	std::vector<GraphNode*> Nodes;
};
