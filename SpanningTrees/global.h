#pragma once
#include <string>

struct Node {
	std::string name;
	int length = 0;
};

struct Set {
	std::string * keys;
};

struct Edge {
	std::string vertex1;
	std::string vertex2;
	double weight;
};