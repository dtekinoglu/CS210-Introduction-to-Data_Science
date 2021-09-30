#ifndef MINHEAP_H
#define MINHEAP_H
#include <vector>
#include <string>

using namespace std;


//heap element consist of blockID and number of reserved seats.
struct blocksRecord{
	int blockID;
	int reserved;
};


class minHeap {
	public:
		minHeap();
		~minHeap();
		blocksRecord getMin();
		blocksRecord remove(int pos);
		void insert(blocksRecord temp);
		int reserveSeatByRow();
		void printHeap();
		void reserveByFull(int b_ID);
		void cancel(int b_ID);
	private:
		vector<blocksRecord> heap;
		vector<int> location;  //array to keep locations of each block in heap.
		void percolateDown(int i);
		void percolateUp(int i);
		int currentSize;
};

#endif