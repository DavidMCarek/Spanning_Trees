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
	Edge * edges;
	Set * sets;
	void unionSets(int index1, int index2);
	bool fullTree = false;
	void printTree(int edgesUsed);
	void addTreeEdge(std::string vertex1, std::string vertex2);
private:
	Edge * treeEdges;
	int finalEdges = 0;
	std::string strFix(std::string in, int length);
	int edgeCount;
	int setSize;
	int setCount = 0;
	void sortEdges(int edgeCount);
};

