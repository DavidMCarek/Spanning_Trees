// EECS:2510:Lab5:Minimum Weight Spanning Trees
//David Carek

#include "stdafx.h"
#include <string>
#include <fstream>
#include <iostream>
#include "global.h"
#include "Kruskal.h"

using namespace std;

// this is the struct for the nodes in heap used in prim's algorithm
struct HeapNode
{
	int key;
	double weight;
	int parent;
};

// returns the integer location of the key in the heap. -1 if not found
int isInHeap(int key, int heapSize, HeapNode * heap);

// gets the minimum weight node from the heap
HeapNode extractMin(HeapNode * heap, double ** adjacencyMatrix, int heapSize);

// sorts the heap recursively
void minHeapify(HeapNode * heap, int heapPosition, int heapSize);

int main()
{
	string inputFilePath = "C:\\Users\\DMCar\\Desktop\\input.txt";

	// will be used to hold names of the vertices
	Vertex * vertices;

	// will hold the adjacency matrix from the input file
	double ** adjacenyMatrix;

	std::ifstream inputStream;
	inputStream.open(inputFilePath, std::ios::binary); // binary flag is set to read the file one byte at a time

	// if we couldn't open the file, let the user know and return
	if (inputStream.fail())
	{
		cout << "Could not open file" << endl;
		cout << "Processing finished. Press ENTER to exit" << endl;
		char waitChar;
		cin.get(waitChar);
		return 0;
	}

	char nextChar = 0; // next character from file
	string input = ""; // string that holds all nextChars until ready to be stored or converted

	inputStream.get(nextChar);

	// until we reach a delimeter (carriage return, line feed, space) keep getting the next character 
	while (nextChar != 10 && nextChar != 13 && nextChar != 32)
	{
		input.push_back(nextChar);
		inputStream.get(nextChar);
	}

	// convert the first number of the file to the total number of nodes
	int numberOfNodes = stoi(input);

	// create an array to hold all vertices
	vertices = new Vertex[numberOfNodes];

	// create adjacency matrix
	adjacenyMatrix = new double *[numberOfNodes];
	for (int i = 0; i < numberOfNodes; i++)
		adjacenyMatrix[i] = new double[numberOfNodes];

	int index = 0;

	// until we get all vertex names keep getting the next char of the file
	// if its not a delimeter. if it is a delimeter add that char to the vertex name
	while (index < numberOfNodes)
	{
		inputStream.get(nextChar);
		if (nextChar == 10 || nextChar == 13 || nextChar == 32)
		{
			if (vertices[index].name.length() != 0)
				index++;
		}
		else
		{
			vertices[index].name.push_back(nextChar);
			vertices[index].length++;
		}
			
	}

	int row = 0;
	int col = 0;
	input = "";
	double matrixValue = 0;
	int edgeCount = 0;

	// keep getting the rest of the characters in the file to build the adjacency matrix
	while (true)
	{
		inputStream.get(nextChar);

		if (inputStream.eof())
			break;
		
		// if next char is a space and the input not empty set the corresponding matrix value from the
		// string and increment the col counter for the matrix value
		if (nextChar == 32)
		{
			if (input != "")
			{
				matrixValue = stod(input);
				if (matrixValue != 0)
					edgeCount++;

				adjacenyMatrix[row][col] = matrixValue;

				col++;
				input = "";
			}
			
		}
		// else if the next char is carriage return or line feed and the input not empty set the corresponding 
		// matrix value from the string input and now we have reached a new line so we start a new row
		else if (nextChar == 10 || nextChar == 13)
		{
			if (input != "")
			{
				matrixValue = stod(input);

				if (matrixValue != 0)
					edgeCount++;

				adjacenyMatrix[row][col] = matrixValue;
				input = "";
			
				row++;
				col = 0;
			}
			
		}
		else // else add the character to the string input
		{
			input.push_back(nextChar);
		}
	}

	// we need to correct the edge count since duplicates were counted
	edgeCount = edgeCount / 2;

	// if we reached the end of the file we might have a leftover value in the input
	if (input != "")
	{
		adjacenyMatrix[numberOfNodes - 1][numberOfNodes - 1] = stod(input);
	}


	//////////Kruskal's//////////


	// initialize kruskal with the set size needed
	Kruskal kruskal(numberOfNodes);

	// create the edges of the graph
	kruskal.buildEdges(adjacenyMatrix, vertices, edgeCount, numberOfNodes);

	int numberOfEdgesUsed = 0;

	// initialize sets for all vertices
	for (int i = 0; i < numberOfNodes; i++)
		kruskal.makeSet(vertices[i].name);

	// for each edge get the sets with the corresponding vertices 
	for (int i = 0; i < edgeCount; i++)
	{
		int setIndex1 = kruskal.findSetIndex(kruskal.edges[i].vertex1);
		int setIndex2 = kruskal.findSetIndex(kruskal.edges[i].vertex2);
		// if the sets are not the same then they need to be unioned and the edge added to the list
		// minimum weigth edges
		if (setIndex1 != setIndex2)
		{
			kruskal.unionSets(setIndex1, setIndex2);
			kruskal.addTreeEdge(kruskal.edges[i].vertex1, kruskal.edges[i].vertex2);
			numberOfEdgesUsed++;
		}
		// once the tree is full all sets have been unioned and the edge count is one off so it is adjusted
		if (kruskal.fullTree)
		{
			numberOfEdgesUsed--;
			break;
		}
	}

	// the edges and weights are printed
	cout << "Kruskal's Algorithm" << endl;
	kruskal.printTree(numberOfEdgesUsed);


	//////////Prim's//////////
	

	cout << endl << "Prim's Algorithm" << endl;
	HeapNode * minHeap = new HeapNode[numberOfNodes + 1];
	int heapSize = 0;

	// add each vertex to the heap
	for (int i = 1; i <= numberOfNodes; i++)
	{
		minHeap[i].key = i - 1;
		minHeap[i].weight = DBL_MAX;
		minHeap[i].parent = -1;
		heapSize++;
	}

	// set the first nodes weight to 0 making its weight the lowest
	minHeap[1].weight = 0;

	HeapNode vertex1;
	HeapNode vertex2;

	// while the heap is not empty, extract the lowest weight node and find its adjacent vertices
	while (heapSize > 0)
	{
		vertex1 = extractMin(minHeap, adjacenyMatrix, heapSize);

		heapSize--;
		for (int i = 0; i < numberOfNodes; i++)
		{
			// value is non zero for edges
			if (adjacenyMatrix[vertex1.key][i] != 0)
			{
				vertex2.key = i;
				int heapLocation = isInHeap(vertex2.key, heapSize, minHeap);

				// if heap location = -1 then the key was not found in the heap
				if (heapLocation > -1)
				{
					vertex2.weight = minHeap[heapLocation].weight;
					double weight = adjacenyMatrix[vertex1.key][vertex2.key];

					// only edit the vertex in the heap if the weight just discovered for it is less than 
					// its previous weight in the heap
					if (weight < vertex2.weight)
					{
						vertex2.parent = vertex1.key;
						vertex2.weight = adjacenyMatrix[vertex1.key][vertex2.key];
						for (int j = 1; j <= numberOfNodes; j++)
							// save the corresponding vertex to the heap and fix the heap after the changes
							if (minHeap[j].key == vertex2.key)
							{
								minHeap[j] = vertex2;
								// since the node changed might be in the middle of the tree we need to run min heapify
								// to fix anything below and above. so we keep going up the heap to fix potential messups
								int heapPosition = j;
								while (heapPosition > 0)
								{
									minHeapify(minHeap, heapPosition, heapSize);
									heapPosition = floor(heapPosition / 2.0);
								}
								break;
							}
					}
				}
			}
		}
	}

	numberOfEdgesUsed = 0;
	Edge * edges = new Edge[numberOfNodes - 1];
	
	// for each vertex that has a parent after prim's has run create edges using the vertex key
	// and parent to get the name of the vertices that make the edge
	for (int i = 0; i < numberOfNodes; i++)
	{
		if (minHeap[i + 1].parent != -1)
		{
			Edge edge;
			edge.vertex1 = vertices[minHeap[i + 1].key].name;
			edge.vertex2 = vertices[minHeap[i + 1].parent].name;
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

	// use bubble sort to alphabetize edges
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

	// print out the edges while calculating the weight of the tree
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

// returns the location of the key in the heap. -1 if not found
int isInHeap(int key, int heapSize, HeapNode * heap)
{
	for (int i = 1; i <= heapSize; i++)
		if (heap[i].key == key)
			return i;

	return -1;
}

// recursively corrects the min heap
void minHeapify(HeapNode * heap, int heapPosition, int heapSize)
{
	// set the left and right children of the current node
	int left = (heapPosition * 2);
	int right = (heapPosition * 2) + 1;

	int min;

	// get the minimum of the three nodes
	if (left <= heapSize && heap[left].weight < heap[heapPosition].weight)
		min = left;
	else 
		min = heapPosition;

	if (right <= heapSize && heap[right].weight < heap[min].weight)
		min = right;
	
	// if the min wasn't at the root of the sub heap swap the root with the min and then call
	// heapify again
	if (min != heapPosition)
	{
		HeapNode tempNode;
		tempNode = heap[heapPosition];
		heap[heapPosition] = heap[min];
		heap[min] = tempNode;
		minHeapify(heap, min + 1, heapSize);
	}
}

// retrieve the smallest weight node in the heap 
HeapNode extractMin(HeapNode * heap, double ** adjacencyMatrix, int heapSize)
{
	// the first item is the min
	HeapNode min = heap[1];
	// swap the min and the last position
	heap[1] = heap[heapSize];
	heap[heapSize] = min;
	// decrease the heap size
	heapSize--;
	minHeapify(heap, 1, heapSize);
	return min;
}
