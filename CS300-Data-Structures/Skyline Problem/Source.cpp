#include "MPQ.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <functional>
#include <queue>
using namespace std;


vector<vector<int>> y_coords; //corresponding heights of x-coordinates that are either start or end of a building.
int n; //number of buildings.

//functions for sorting x_coords list using heapSort.
bool myOrder(vector<int>,vector<int>); //sorting x-coordinates in particular order(ascending but also covering edge cases where Xs are equal.)
void percolateDown(int, vector<vector<int>>& ); //for maintaining heap order property.
void buildHeap(vector<vector<int>> & ); //getting all values to heap.
void deleteMin(vector<int> & , vector<vector<int>> &); //getting min in the heap
vector<vector<int>> heapSort(vector<vector<int>> & ); //sorting function.



int main() {


	string file = "input.txt";
	ifstream in;
	in.open(file.c_str());
	string firstLine, currentLine;
	getline(in,firstLine);
	istringstream number(firstLine);
	number >>n;

	// If no building present, at x = 0
	if (n == 0) cout << "0 0" << endl;

	//If number of building is invalid.
	if (n <0) return 0;

	int left, right, top;
	int label = 1; //IDs of buildings.

	vector<vector<int>> x_coords;
	vector<int>temp;
	vector<int> dummy;

	dummy.push_back(-1);
	dummy.push_back(-1);
	y_coords.push_back(dummy);

	// getting all x-coordinates to x_coords list and heights to y_coords list together with building IDs.
	while (in.eof()==false) {
		getline(in,currentLine);
		istringstream cur(currentLine);
		cur>>left>>top>>right;

		temp.push_back(left);
		temp.push_back(label);
		temp.push_back(0);
		x_coords.push_back(temp); //pushing start of building.

		temp.clear();

		temp.push_back(right);
		temp.push_back(label);
		temp.push_back(1);
		x_coords.push_back(temp); //pushing end of building.

		temp.clear();

		temp.push_back(top);
		temp.push_back(label);
		y_coords.push_back(temp); //pushing height of corresponding ID.

		temp.clear();

		label++; //incrementing ID.
	}
	
	//sorting x-coordinates of start/end of buildings in ascending order.
	x_coords = heapSort(x_coords);

	if (x_coords[0][0] != 0) cout << 0 << " " <<0 << endl;


	MPQ heap;


	// whenever you encounter a start of building, 
	//push the height of that building to priority queue. 
	//If max is changed,  it means that height is greater.

	// whenever you encounter an end of building, 
	//remove from heap. if max height is changed
	//it means that this value must be part of final answer.
	
	int h, lab;
	int cur_max = 0;
	for (int i = 0; i < x_coords.size(); i++){

		if (!heap.isEmpty())
			cur_max = heap.getMax().data;
		if (x_coords[i][2] == 0)  {//it is a start
			h = y_coords[x_coords[i][1]][0];
			lab = x_coords[i][1];
			heap.insert(h, lab);
		}
		else 
			heap.remove(x_coords[i][1]);
		if (heap.getMax().data != cur_max) {
				cur_max = heap.getMax().data;
				cout << x_coords[i][0] << " " << cur_max << endl;
		}

	}


	return 0;
}




void buildHeap(vector<vector<int>> & x_coords){
	 for(int i = ((x_coords.size()-1) / 2); i >= 0; i--)
		 percolateDown(i, x_coords);
}

void percolateDown(int i, vector<vector<int>> & x_coords){
	if (x_coords.size() <1) return;
	int child;
	vector<int> temp = x_coords[i]; //item to be percolated down.
	for(;i*2+1<x_coords.size(); i = child){
		child = i*2+1;
		if (child !=x_coords.size()-1 && myOrder(x_coords[child+1], x_coords[child])==false) child++;
		if (myOrder(x_coords[child], temp) == false) x_coords[i] = x_coords[child];
		else break;
	}
	x_coords[i] = temp;
}


//deleting min item from the heap.
void deleteMin(vector<int> & minItem, vector<vector<int>> & x_coords ){
	if (x_coords.size()!=0){
		minItem = x_coords[0];
		for (int i = 0; i < 3; i++) {
			x_coords[0][i] = x_coords[x_coords.size()-1][i];
		}
		x_coords.pop_back();
		percolateDown(0, x_coords);
	}
}


// Sorting x_coordinates to be examined in ascending order.
vector<vector<int>> heapSort(vector<vector<int>> & x_coords){
	buildHeap(x_coords);
	vector<int> minItem;
	vector<vector<int>> newList;
	while (x_coords.size()!=0){
		deleteMin(minItem,x_coords);
		newList.push_back(minItem);
	}
	return newList;

}


bool myOrder(vector<int> left, vector<int>right){
	if (left[0] > right[0]) return true;
	if (left[0] < right[0]) return false;

	if (left[0] == right[0]){ //if x_coordinates are equal.
	
		if (left[2] == 0 && right[2]==0) {//edge case 1: if two buildings start at the same point, start processing from higher to lower.
		   if (y_coords[left[1]] < y_coords[right[1]])  return true; 	
			else return false;
	   }

	   if (left[2] == 1 && right[2] == 1) {//edge case 2: if two buildings end at the same point, start processing from lower one.
		   if (y_coords[left[1]] > y_coords[right[1]])  return true; //5-LINE RULE
			else return false;
	   }
	   if (left[2] == 1 && right[2] == 0) {//edge case 3: if end of a building is the same with the start of another one, next building should be examined first.
			return true;
	   }
	   else if (left[2] == 0 && right[2] == 1)//edge case 3: if end of a building is the same with the start of another one, next building should be examined first.
		   return false;
	
	}
	return false;
}
