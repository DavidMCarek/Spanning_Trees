#pragma once
#include <string>

class MinHeap
{
public:
	MinHeap();
	~MinHeap();
	void insert(std::string input);
private:
	struct Node {
		std::string key = "";
		Node * leftChild = nullptr;
		Node * rightChild = nullptr;
	};
	Node * root = nullptr;
};

