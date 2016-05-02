#include "stdafx.h"
#include "Kruskal.h"
#include "global.h"
#include <iostream>

Kruskal::Kruskal(int setSize)
{
	sets = new Set[setSize];
	this->setSize = setSize;
}


Kruskal::~Kruskal()
{
}

void Kruskal::buildEdges(double ** adjacencyMatrix, Node * nodes, int edgeCount, int nodeCount)
{
	this->edgeCount = edgeCount;

	edges = new Edge[edgeCount];

	int edgeIndex = 0;

	for (int i = 0; i < nodeCount; i++)
	{
		for (int j = 0; j < nodeCount; j++)
		{
			if (adjacencyMatrix[i][j] != 0)
			{
				Edge edge;
				edge.weight = adjacencyMatrix[i][j];
				
				if (nodes[i].name.compare(nodes[j].name) < 0)
				{
					edge.vertex1 = strFix(nodes[i].name, nodes[i].length);
					edge.vertex2 = strFix(nodes[j].name, nodes[j].length);
				}
				else
				{
					edge.vertex1 = strFix(nodes[j].name, nodes[j].length);
					edge.vertex2 = strFix(nodes[i].name, nodes[i].length);
				}

				edges[edgeIndex] = edge;
				edgeIndex++;
			}
		}
	}

	sortEdges(edgeCount);
}

void Kruskal::sortEdges(int edgeCount)
{
	Edge tempEdge;
	for (int i = 1; i < edgeCount; i++)
	{
		if (edges[i].weight < edges[i - 1].weight)
		{
			tempEdge = edges[i];
			edges[i] = edges[i - 1];
			edges[i - 1] = tempEdge;
		}
	}
}

void Kruskal::makeSet(std::string vertex)
{
	sets[setCount].keys = new std::string[setSize];
	sets[setCount].keys[0] = vertex;
	setCount++;
}

int Kruskal::findSetIndex(std::string vertex)
{
	for (int i = 0; i < setSize; i++)
	{
		for (int j = 0; j < setSize; j++)
		{
			if (sets[i].keys[j].compare(vertex) == 0)
				return i;
		}
	}
}

void Kruskal::unionSets(int setIndex1, int setIndex2)
{
	int firstEmpty = -1;
	for (int i = 0; i < setSize; i++)
	{
		if (sets[setIndex1].keys[i].length() == 0)
		{
			firstEmpty = i;
			break;
		}
	}

	if (firstEmpty == -1)
	{
		fullTree = true;
		return;
	}

	for (int i = firstEmpty; i < setSize; i++)
	{
		sets[setIndex1].keys[i] = sets[setIndex2].keys[i - firstEmpty];
		sets[setIndex2].keys[i - firstEmpty] = "";
	}
}

void Kruskal::printTree(int edgesUsed)
{
	Edge * tempEdges = new Edge[edgesUsed];

	for (int i = 0; i < edgeCount; i++)
	{
		tempEdges[i] = edges[i];

		std::cout << edges[i].vertex1 << " - " << edges[i].vertex2 << std::endl;
	}

	Edge tempEdge;

	for (int i = 1; i < edgesUsed; i++)
	{
		
	}
}

std::string Kruskal::strFix(std::string in, int length)
{
	std::string out = "";
	for (int i = 0; i < length; i++)
	{
		out.push_back(in[i]);
	}
	return out;
}