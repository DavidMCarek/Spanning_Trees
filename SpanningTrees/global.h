#pragma once
#include <string>

struct Node {
	std::string name;
	int length = 0;
};

struct Set {
	std::string * keys;
};

std::string strFix(std::string in, int length)
{
	std::string out = "";
	for (int i = 0; i < length; i++)
	{
		out.append(in[i] + "");
	}
	return out;
}
