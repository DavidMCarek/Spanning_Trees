#pragma once
#include <string>
#include "global.h"

class Kruskal
{
public:
	Kruskal(int setSize);
	~Kruskal();
	void buildEdges(double ** adjacencyMatrix, Node * nodes, int edgeCount, int nodeCount);
	void makeSet(std::string vertex);
	int findSetIndex(std::string vertex);
	struct Edge {
		std::string vertex1;
		std::string vertex2;
		double weight;
	};
	Edge * edges;
	Set * sets;
	void unionSets(int index1, int index2);
	bool fullTree = false;
	void printTree(int edgesUsed);
private:
	int edgeCount;
	int setSize;
	int setCount = 0;
	void sortEdges(int edgeCount);
};

