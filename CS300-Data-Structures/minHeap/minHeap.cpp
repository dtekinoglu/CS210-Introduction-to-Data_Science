#include "minHeap.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

extern int b,c,r; //number of buildings.

extern vector<string> blockList;

// constructor of modified priority queue.
minHeap::minHeap (){

	//dummy head for the heap.
	blocksRecord newBlock;
	newBlock.blockID = -1;
	newBlock.reserved = -1;
	heap.push_back(newBlock);
	for (int i = 1 ; i <b+1; i++){
		newBlock.blockID = i;
		newBlock.reserved = 0;
		heap.push_back(newBlock);
	}
	location.push_back(0);
	for (int i = 1 ; i <b+1; i++){
		location.push_back(i);
	}

	currentSize = b-1;

}


// destructor of modified priority queue.
minHeap::~minHeap (){
	heap.clear();
	location.clear();
}



// returns min. element of minHeap. (second element in heap_array because of dummy head).
blocksRecord minHeap::getMin(){
	return heap[1]; 
}




//removing element from heap.
blocksRecord minHeap::remove (int ID){
	blocksRecord t = heap[location[ID]];
	location[heap[currentSize].blockID] = location[ID];
	heap[location[ID]] = heap[currentSize--];
	percolateDown(location[ID]);
	return t;

}

//for maintaining heap order property after deletion.
void minHeap::percolateDown(int i){ 
	int child;
	blocksRecord tmp = heap[i];
	for( ;i * 2 <= currentSize; i = child ) {
       child = i * 2;
	   if (child != currentSize && (heap[child+1].reserved < heap[child].reserved)|| (heap[child+1].reserved ==  heap[child].reserved && heap[child+1].blockID < heap[child].blockID))
           child++;
	   if(heap[child].reserved < tmp.reserved || (heap[child].reserved == tmp.reserved && heap[child].blockID < tmp.blockID))
	   {
           heap[i] = heap[child];
		   location[heap[child].blockID] = i;
	   }
       else {
		   break;}
    }
	heap[i] = tmp;
	location[tmp.blockID] = i;
}


//inserting element to the heap.
void minHeap::insert(blocksRecord temp){

	int hole = ++currentSize;

	for (; hole>1 && (temp.reserved <heap[hole/2].reserved || (temp.reserved == heap[hole/2].reserved && temp.blockID < heap[hole/2].blockID));hole/=2){
		location[heap[hole/2].blockID] *=2;
		heap[hole] = heap[hole/2];

	}
	heap[hole] = temp;
	location[temp.blockID] = hole;


}


// when a reservation cancelled, related element in the heap must be percolated up.
void minHeap::percolateUp(int hole){
	blocksRecord temp = heap[hole];
	for (; hole>1 && (temp.reserved <heap[hole/2].reserved)|| (temp.reserved == heap[hole/2].reserved && temp.blockID < heap[hole/2].blockID);hole/=2){
		location[heap[hole/2].blockID] *=2;
		heap[hole] = heap[hole/2];
	}
	heap[hole] = temp;
	location[temp.blockID] = hole;


}


//reserving by emptiest block.
int minHeap::reserveSeatByRow(){
	heap[1].reserved++;
	int id = heap[1].blockID;
	percolateDown(1);
	return id;
}



//reservation by entire seat info
void minHeap::reserveByFull(int b_ID){
	heap[location[b_ID]].reserved++;
	percolateDown(location[b_ID]);
}


//cancelling reservation
void minHeap::cancel(int b_ID){
	heap[location[b_ID]].reserved--;
	percolateUp(location[b_ID]);
}


//prints heap
void minHeap::printHeap(){
	for (int i = 1; i <=currentSize; i++)
		cout << "name: " <<blockList[heap[i].blockID] << " id: " << heap[i].blockID << " num: " << heap[i].reserved <<endl;
	cout << endl;
	for(int i = 1; i < location.size();i++) cout << location[i] << " ";
	cout << endl <<endl;
}