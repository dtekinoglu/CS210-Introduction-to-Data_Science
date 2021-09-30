#ifndef MPQ_H
#define MPQ_H
#include <vector>

using namespace std;

extern int n; //number of buildings.
struct queueNode{
	int data;
	int ID;
};

class MPQ {
	public:
		MPQ();
		~MPQ();
		bool isEmpty();
		queueNode getMax();
		queueNode remove(int label);
		void insert(int value, int label);
		void printHeap();
	private:
		int current_size;
		vector<queueNode> heap;
		vector<int> location;  //array to keep locations of each ID in heap.
		void percolateDown(int i);
};

#endif