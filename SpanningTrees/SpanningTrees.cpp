// SpanningTrees.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <fstream>
#include <iostream>
#include "global.h"
#include "Kruskal.h"

using namespace std;

std::string strFix(std::string in, int length);

struct HeapNode
{
	string key;
	double weight;
};

int main()
{
	string inputFilePath = "C:\\Users\\DMCar\\Desktop\\input.txt";

	Node * nodes;

	double ** adjacenyMatrix;

	std::ifstream inputStream;
	inputStream.open(inputFilePath, std::ios::binary); // binary flag is set to read the file one byte at a time

													   // if we couldn't open the file, let the user know and return
	if (inputStream.fail())
	{
		cout << "Could not open file" << endl;
		return 0;
	}

	int numberOfNodes = 0;
	char nextChar = 0;
	string input = "";

	inputStream.get(nextChar);
	while (nextChar != 10 && nextChar != 13 && nextChar != 32)
	{
		input.push_back(nextChar);
		inputStream.get(nextChar);
	}

	int n = stoi(input);
	nodes = new Node[n];

	adjacenyMatrix = new double *[n];
	for (int i = 0; i < n; i++)
		adjacenyMatrix[i] = new double[n];

	int index = 0;

	while (index < n)
	{
		inputStream.get(nextChar);
		if (nextChar == 10 || nextChar == 13 || nextChar == 32)
		{
			if (nodes[index].name.length() != 0)
				index++;
		}
		else
		{
			nodes[index].name.push_back(nextChar);
			nodes[index].length++;
		}
			
	}

	int row = 0;
	int col = 0;
	string strVal = "";
	double val = 0;
	int edgeCount = 0;

	while (true)
	{
		inputStream.get(nextChar);

		if (inputStream.eof())
			break;
		
		if (nextChar == 32) // if next char is a space 
		{
			if (strVal != "")
			{
				val = stod(strVal);
				if (val != 0)
					edgeCount++;

				adjacenyMatrix[row][col] = val;

				col++;
				strVal = "";
			}
			
		}
		else if (nextChar == 10 || nextChar == 13)
		{
			if (strVal != "")
			{
				val = stod(strVal);

				if (val != 0)
					edgeCount++;

				adjacenyMatrix[row][col] = val;
				strVal = "";
			
				row++;
				col = 0;
			}
			
		}
		else
		{
			strVal.push_back(nextChar);
		}
	}

	edgeCount = edgeCount / 2;

	if (strVal != "")
	{
		adjacenyMatrix[n - 1][n - 1] = stod(strVal);
	}

	Kruskal kruskal(n);
	kruskal.buildEdges(adjacenyMatrix, nodes, edgeCount, n);

	int numberOfEdgesUsed = 0;

	for (int i = 0; i < n; i++)
		kruskal.makeSet(strFix(nodes[i].name, nodes[i].length));

	for (int i = 0; i < edgeCount; i++)
	{
		int setIndex1 = kruskal.findSetIndex(kruskal.edges[i].vertex1);
		int setIndex2 = kruskal.findSetIndex(kruskal.edges[i].vertex2);
		if (setIndex1 != setIndex2)
		{
			kruskal.unionSets(setIndex1, setIndex2);
			kruskal.addTreeEdge(kruskal.edges[i].vertex1, kruskal.edges[i].vertex2);
			numberOfEdgesUsed++;
		}
		if (kruskal.fullTree)
		{
			numberOfEdgesUsed--;
			break;
		}
	}

	cout << "Kruskal's Algorithm" << endl;
	kruskal.printTree(numberOfEdgesUsed);


	

	



	
	inputStream.close();

    return 0;
}

std::string strFix(std::string in, int length)
{
	std::string out = "";
	for (int i = 0; i < length; i++)
	{
		out.push_back(in[i]);
	}
	return out;
}