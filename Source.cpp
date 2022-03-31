// Autor: Valery Sokolov Morgenshteyn ID: 336424221
#include <iostream>
#include <fstream>
#include "SmartReader.h"
int main()
{
	std::ofstream out("output.txt");
	if (!out)
	{
		std::cout << "Can't open input.txt";
		exit(1);
	}
	std::ifstream in("input.txt");
	if (!in)
	{
		out << "Can't open input.txt";
		exit(1);
	}
	
	SmartReader smart(&in, &out);
	smart.BuildFile();
	in.close();
	out.close();
}