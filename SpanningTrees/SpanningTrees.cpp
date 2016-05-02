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
	int key;
	double weight;
	int parent;
};

int isInHeap(int key, int heapSize, HeapNode * heap);
HeapNode extractMin(HeapNode * heap, double ** adjacencyMatrix, int heapSize);
void minHeapify(HeapNode * heap, int heapPosition, int heapSize);

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
	cout << endl << "Prim's Algorithm" << endl;

	

	HeapNode * minHeap = new HeapNode[n + 1];
	int heapSize = 0;

	for (int i = 1; i <= n; i++)
	{
		minHeap[i].key = i - 1;
		minHeap[i].weight = DBL_MAX;
		minHeap[i].parent = -1;
		heapSize++;
	}

	minHeap[1].weight = 0;

	HeapNode vertex1;
	HeapNode vertex2;

	while (heapSize > 0)
	{
		vertex1 = extractMin(minHeap, adjacenyMatrix, heapSize);

		heapSize--;
		for (int i = 0; i < n; i++)
		{
			if (adjacenyMatrix[vertex1.key][i] != 0)
			{
				vertex2.key = i;
				int heapLocation = isInHeap(vertex2.key, heapSize, minHeap);
				if (heapLocation > -1)
				{
					vertex2.weight = minHeap[heapLocation].weight;
					double weight = adjacenyMatrix[vertex1.key][vertex2.key];
					if (weight < vertex2.weight)
					{
						vertex2.parent = vertex1.key;
						vertex2.weight = adjacenyMatrix[vertex1.key][vertex2.key];
						for (int j = 1; j <= n; j++)
							if (minHeap[j].key == vertex2.key)
							{
								minHeap[j] = vertex2;
								minHeapify(minHeap, 1, heapSize);
							}
					}
				}
			}
		}
	}


	numberOfEdgesUsed = 0;
	Edge * edges = new Edge[n - 1];
	for (int i = 0; i < n; i++)
	{
		if (minHeap[i + 1].parent != -1)
		{
			Edge edge;
			edge.vertex1 = strFix(nodes[minHeap[i + 1].key].name, nodes[minHeap[i + 1].key].length);
			edge.vertex2 = strFix(nodes[minHeap[i + 1].parent].name, nodes[minHeap[i + 1].parent].length);
			if (edge.vertex1 > edge.vertex2)
			{
				string tempStr = edge.vertex1;
				edge.vertex1 = edge.vertex2;
				edge.vertex2 = tempStr;
			}

			edge.weight = minHeap[i + 1].weight;
			edges[numberOfEdgesUsed] = edge;
			numberOfEdgesUsed++;
		}
	}

	Edge tempEdge;

	for (int passes = 0; passes < numberOfEdgesUsed - 1; passes++)
		for (int i = 1; i < numberOfEdgesUsed; i++)
		{
			if ((edges[i].vertex1 + edges[i].vertex2) < (edges[i - 1].vertex1 + edges[i - 1].vertex2))
			{
				tempEdge = edges[i];
				edges[i] = edges[i - 1];
				edges[i - 1] = tempEdge;
			}
		}

	double weight = 0;
	for (int i = 0; i < numberOfEdgesUsed; i++)
	{
		weight += edges[i].weight;
		std::cout << edges[i].vertex1 << " - " << edges[i].vertex2 << " : " << edges[i].weight << std::endl;
	}

	cout << weight << endl << endl;
	
	cout << "Processing finished. Press ENTER to exit" << endl;
	char waitChar;
	cin.get(waitChar);
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

int isInHeap(int key, int heapSize, HeapNode * heap)
{
	for (int i = 1; i <= heapSize; i++)
		if (heap[i].key == key)
			return i;

	return -1;
}

void minHeapify(HeapNode * heap, int heapPosition, int heapSize)
{
	int left = (heapPosition * 2);
	int right = (heapPosition * 2) + 1;

	int min;
	if (left <= heapSize && heap[left].weight < heap[heapPosition].weight)
		min = left;
	else 
		min = heapPosition;

	if (right <= heapSize && heap[right].weight < heap[min].weight)
		min = right;
	
	if (min != heapPosition)
	{
		HeapNode tempNode;
		tempNode = heap[heapPosition];
		heap[heapPosition] = heap[min];
		heap[min] = tempNode;
		minHeapify(heap, min + 1, heapSize);
	}
}

HeapNode extractMin(HeapNode * heap, double ** adjacencyMatrix, int heapSize)
{
	HeapNode min = heap[1];
	heap[1] = heap[heapSize];
	heap[heapSize] = min;
	heapSize--;
	minHeapify(heap, 1, heapSize);
	return min;
}
