#include "stdafx.h"
#include "Kruskal.h"
#include "global.h"
#include <iostream>

// when creating krukal create the sets array
Kruskal::Kruskal(int setSize)
{
	sets = new Set[setSize];
	this->setSize = setSize;
}


Kruskal::~Kruskal()
{
}

// creates the edges and sorts on weight for the sets
void Kruskal::buildEdges(double ** adjacencyMatrix, Vertex * vertices, int edgeCount, int nodeCount)
{
	this->edgeCount = edgeCount;

	edges = new Edge[edgeCount];
	treeEdges = new Edge[edgeCount];

	int edgeIndex = 0;

	// for each vertex pair with a nonzero weight create the edge between them
	for (int i = 0; i < nodeCount; i++)
	{
		for (int j = 0; j < nodeCount; j++)
		{
			// this avoids adding duplicate edges
			if (j > i)
				continue;

			// if the weight is nonzero create the edge for the vertices
			if (adjacencyMatrix[i][j] != 0)
			{
				Edge edge;
				edge.weight = adjacencyMatrix[i][j];
				
				if (vertices[i].name.compare(vertices[j].name) < 0)
				{
					edge.vertex1 = vertices[i].name;
					edge.vertex2 =vertices[j].name;
				}
				else
				{
					edge.vertex1 = vertices[j].name;
					edge.vertex2 =vertices[i].name;
				}

				edges[edgeIndex] = edge;

				edgeIndex++;

			}
		}
	}

	// sort the edges created by weight
	sortEdges(edgeCount);
}

// bubble sort the edgesin kruskal
void Kruskal::sortEdges(int edgeCount)
{
	Edge tempEdge;
	// make edgecount - 1 passes through the list swapping edges that are inverted
	for (int passes = 0; passes < edgeCount - 1; passes++)
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

// initailizes each set of one vertex
void Kruskal::makeSet(std::string vertex)
{
	sets[setCount].keys = new std::string[setSize];
	sets[setCount].keys[0] = vertex;
	setCount++;
}

// returns the index for a set when the vertex is found
int Kruskal::findSetIndex(std::string vertex)
{
	std::string setVertex = "";
	for (int i = 0; i < setSize; i++)
	{
		for (int j = 0; j < setSize; j++)
		{
			setVertex = sets[i].keys[j];
			if (setVertex.compare(vertex) == 0)
				return i;
		}
	}
}

// adds an edge to the list of final tree edges
void Kruskal::addTreeEdge(std::string vertex1, std::string vertex2)
{
	for (int i = 0; i < edgeCount; i++)
	{
		if (edges[i].vertex1 == vertex1 && edges[i].vertex2 == vertex2)
		{
			treeEdges[finalEdges] = edges[i];
			finalEdges++;
			return;
		}
	}
}

// joins two sets based on index
void Kruskal::unionSets(int setIndex1, int setIndex2)
{
	int firstEmpty = -1;
	// find the first location we can insert into
	for (int i = 0; i < setSize; i++)
	{
		if (sets[setIndex1].keys[i].length() == 0)
		{
			firstEmpty = i;
			break;
		}
	}

	// if we cannot insert then our tree is full and kruskals is done
	if (firstEmpty == -1)
	{
		fullTree = true;
		return;
	}

	// move all items from set 2 into set 1
	for (int i = firstEmpty; i < setSize; i++)
	{
		sets[setIndex1].keys[i] = sets[setIndex2].keys[i - firstEmpty];
		sets[setIndex2].keys[i - firstEmpty] = "";
	}
}

// prints the tree in alphabetical order
void Kruskal::printTree(int edgesUsed)
{
	Edge tempEdge;

	// bubble sorts the tree edges by name
	for (int passes = 0; passes < edgesUsed - 1; passes++)
		for (int i = 1; i < edgesUsed; i++)
		{
			if ((treeEdges[i].vertex1 + treeEdges[i].vertex2) < (treeEdges[i - 1].vertex1 + treeEdges[i - 1].vertex2))
			{
				tempEdge = treeEdges[i];
				treeEdges[i] = treeEdges[i - 1];
				treeEdges[i - 1] = tempEdge;
			}
		}

	double weight = 0;
	// prints each edge while calculating the weight
	for (int i = 0; i < edgesUsed; i++)
	{
		weight += treeEdges[i].weight;
		std::cout << treeEdges[i].vertex1 << " - " << treeEdges[i].vertex2 << " : " << treeEdges[i].weight << std::endl;
	}

	std::cout << weight << std::endl;
}