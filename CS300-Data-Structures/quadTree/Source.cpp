#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "quadTree.h"

using namespace std;

int main(){

	string filename = "cities.txt";
	ifstream in;
	in.open(filename.c_str());


	string firstLine;
	getline(in, firstLine); 
	int x ,y;
	istringstream grid(firstLine);
	grid >>x >>y;
	quadTree myTree(x,y);

	string line, cityName;
	int k,m;

	while (in.eof()!= true) { 
		getline(in, line);

		istringstream myLine(line);
		myLine >> cityName;
		myLine >>k>>m;
		myTree.insertCity(cityName, k,m);
	}

	myTree.printTree();

	string query = "queries9.txt";
	string qLine;
	int a, b, r;
	string temp;
	ifstream in_query;
	in_query.open(query.c_str());


	while (in_query.eof() == false){
		getline(in_query, qLine);
		istringstream triple(qLine);
		triple >> a >> temp >> b >>temp >>r;
		myTree.findCities(a,b,r);
		cout << endl << endl;
	}

	return 0;
}