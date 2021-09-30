#include "MPQ.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

extern int n; //number of buildings.

// constructor of modified priority queue.
MPQ::MPQ (){
	queueNode dummy;
	dummy.data = 0;
	dummy.ID = -1;
	for (int i = 0 ; i <n+2; i++){
		heap.push_back(dummy);}
	current_size = 0;
	for (int i = 0 ; i <n+2; i++){
		location.push_back(-1);}
}


// destructor of modified priority queue.
MPQ::~MPQ (){
	heap.clear();
	location.clear();
	current_size = 0;
}

//checks whether the priortiy queue is empty or not.
bool MPQ::isEmpty(){
	return (current_size==0);
}


// returns max. element of queue. (first element in heap_array).
queueNode MPQ::getMax(){
	if (this->isEmpty()) return heap[0];
	return heap[1]; //because of dummy head.
}


//inserting new building to the heap.
void MPQ::insert(int value, int label){
	queueNode building;
	building.data = value;
	building.ID = label;
	int hole = ++current_size;

	for (; hole>1 && value >heap[hole/2].data;hole/=2){
		location[heap[hole/2].ID] =hole; //  1
		heap[hole] = heap[hole/2]; //	2
	}
	heap[hole] = building;
	location[label] = hole;
}


//removing building from heap.
queueNode MPQ::remove (int myID){

	queueNode t = heap[location[myID]];
	location[heap[current_size].ID] = location[myID]; //UST SATIRA ALDIM 3
	heap[location[myID]] = heap[current_size];
	heap[current_size].data = 0;
	heap[current_size].ID = -1;	
	current_size--;
	percolateDown(location[myID]); 
	location[myID] = -1;
	return t;
}

//for maintaining heap order property after deletion.
void MPQ::percolateDown(int i){  
    int child;
    queueNode tmp = heap[i];
	for( ;i * 2 <= current_size; i = child ) {
       child = i * 2;
	   if (child != current_size && heap[child+ 1].data > heap[child].data)
           child++;
	   if(heap[child].data > tmp.data ){
           heap[i] = heap[child];
		   location[heap[child].ID] = i;
	   }
       else
          break;
    }

	heap[i] = tmp;
	if (heap[i].ID >= 0) location[heap[i].ID] = i; //YENÝ		4

}
