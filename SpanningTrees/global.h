#pragma once
#include <string>
// this header contains structs common to all files in solution

struct Vertex {
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