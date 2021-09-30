#include <iostream>
#include <string>
#include "My_heap.h"
#include <iomanip>

using namespace std;

//************************************************
// Constructor. Generates an empty heap         *
//************************************************
My_heap::My_heap(void){
	blk = nullptr;
	heap_begin = nullptr;
    used_bytes = 0;
	usedBlocks = 0;
	unusedBlocks = 0;
}

//************************************************
// Destructor         *
//************************************************
My_heap::~My_heap(){
	memory_block* temp = heap_begin;
	cout <<"At destruction, the heap had a memory leak of " << dec << used_bytes << " bytes" << endl;
	while (temp != nullptr) {
		temp = temp->right;
		delete heap_begin;
		heap_begin = temp;
	}
	used_bytes = 0;
}


memory_block * My_heap::bump_allocate(int num_bytes) {
	if (MAX_CAPACITY-used_bytes <num_bytes) {

		return nullptr;
	}

	if (heap_begin==nullptr){
		heap_begin = new memory_block;
		heap_begin->left = nullptr;
		heap_begin->right = nullptr;
		heap_begin->size = num_bytes;
		used_bytes = num_bytes;
		heap_begin->starting_address = 0;
		heap_begin->used = true;
		blk = heap_begin;

		usedBlocks+=1;

		return heap_begin;
	}
	else{
		memory_block * node = new memory_block;
		node->left =blk;
		node->right = nullptr;
		node->size = num_bytes;
		node->starting_address = blk->starting_address+blk->size;
		blk->right = node;
		blk = node;
		node->used = true;
		used_bytes +=num_bytes;
		usedBlocks+=1;
		return node;
	}

}

void My_heap::deallocate(memory_block* to_delete){
	if (to_delete == nullptr || to_delete->used==false) return; //ADDED LINE

	to_delete->used = false;
	unusedBlocks ++;
	usedBlocks--;
	memory_block * end = to_delete;
	memory_block * start = to_delete;
	int free_bytes = to_delete->size;
	if (to_delete->right != nullptr && to_delete->right->used == false){
		end = to_delete->right;
		unusedBlocks--;
		free_bytes += to_delete->right->size;
	}
	if (to_delete->left != nullptr && to_delete->left->used== false){
		start = to_delete->left;
		unusedBlocks--;
		free_bytes+= to_delete->left->size;	
	}

	memory_block * merged = new memory_block;
	merged->right = end->right;
	if (end->right != nullptr)
		end->right->left = merged;
	else
		blk = merged;
	if (start != heap_begin) {
		merged->left = start->left;
		start->left->right = merged;
	}
	else {
		heap_begin = merged;
		merged->left = nullptr; //ADDED LINE
	}
	merged->size = free_bytes;
	merged->starting_address = start->starting_address;
	merged->used = false;
	used_bytes -= to_delete->size;
	if (used_bytes > MAX_CAPACITY) { //ADDED LINE
	if (end == start)
		delete start;
	else{
		delete start;
		delete end;
	}
	}
}


memory_block * My_heap::first_fit_allocate(int num_bytes){
	if (MAX_CAPACITY-used_bytes <num_bytes) return nullptr; //ADDED LINE
	
	memory_block * temp = heap_begin;
	bool flag = false;
	while (temp!=nullptr){
		if (temp->used == false && temp->size >= num_bytes) {
			flag = true;
			break;
		}
		else
			temp = temp->right;
	}
	if(flag) {
	temp->used = true;
	used_bytes += temp->size;
	usedBlocks++;
	unusedBlocks--;
	return temp;
	}

	else{
		memory_block * result = this->bump_allocate(num_bytes);
		return result;
	}

}


memory_block * My_heap::best_fit_allocate(int num_bytes){

	memory_block * temp = heap_begin;
	memory_block * bestFit = nullptr;
	int currentMin = MAX_CAPACITY;
	while (temp!=nullptr){
		if (temp->used == false && temp->size >= num_bytes && temp->size <=currentMin) {
			currentMin = temp->size;
			bestFit = temp;
		}
		temp = temp->right;
	}
	
	if(bestFit != nullptr) {
		bestFit->used = true;
		used_bytes += bestFit->size;
		usedBlocks++;
		unusedBlocks--;
		return bestFit;
	}
	else{
		memory_block * result = this->bump_allocate(num_bytes);
		return result;
	}
}

memory_block * My_heap::first_fit_split_allocate(int num_bytes) {
	if (MAX_CAPACITY-used_bytes <num_bytes) return nullptr; //ADDED LINE

	memory_block * temp = heap_begin;
	bool flag = false;
	while (temp!=nullptr && flag ==false){
		if (temp->used == false && temp->size >= num_bytes) {
			flag = true;
		}
		else
			temp = temp->right;
	}
	if(flag) {
		if (temp->size == num_bytes) {
			temp->used = true;
			used_bytes += temp->size;
			unusedBlocks--;
			usedBlocks++;
			return temp;
		}
		else{
			memory_block * toBeUsed = new memory_block;
			memory_block * trash = new memory_block;
			if (temp->left !=nullptr) {
			toBeUsed->left = temp->left;
			temp->left->right = toBeUsed;
			}
			else
				heap_begin = toBeUsed;
			toBeUsed->right = trash;
			toBeUsed->used = true;
			toBeUsed->size = num_bytes;
			toBeUsed->starting_address = temp->starting_address;
			trash->left = toBeUsed;
			trash->right = temp->right;
			if (temp != blk)
				temp->right->left = trash;
			trash->size = temp->size - num_bytes;
			trash->used = false;
			trash->starting_address = toBeUsed->starting_address + num_bytes;
			usedBlocks++;
			used_bytes += num_bytes;
			delete temp;
			return toBeUsed;
		}
	}
	else{
		memory_block * result = this->bump_allocate(num_bytes);
		return result;
	}
}

float My_heap::get_fragmentation(){
	float result= 0;
	int freeMemory = MAX_CAPACITY - used_bytes;
	float biggestFree = 0;

	memory_block * temp = heap_begin;
	while (temp != NULL) {
		if (temp->used == false){
			if (temp->size >biggestFree)
				biggestFree = temp->size;
		}
		temp = temp->right;
	}
	int last = MAX_CAPACITY-(blk->starting_address+blk->size);
	if (biggestFree < last)
		biggestFree = last;

	if (biggestFree == 0)
		return result;
	else{
	result= ((freeMemory-biggestFree)/freeMemory)*100;
	return result;
	}
}

string My_heap::occupation(memory_block * block){
	if(block->used == false)
		return "False";
	return "True";
}


void My_heap::print_heap(){
	int q1 = used_bytes;
	
	float frag = this->get_fragmentation();
	
	cout << dec <<"Maximum capacity of heap: 512B\nCurrently used memory (B): " <<q1 <<
		endl <<"Total memory blocks: " << usedBlocks+unusedBlocks <<endl <<
		"Total used memory blocks: " << usedBlocks <<
		endl << "Total free memory blocks: " << unusedBlocks<<endl <<
		"Fragmentation: " << frag <<"%" << endl;

	memory_block * current = heap_begin;
	int i = 0;
	string oc;
	while (current!=nullptr){
		oc = occupation(current);
		cout << "Block " <<  dec << i <<"\t\tUsed: " << oc <<"\tSize (B): " <<  dec << current->size << "\tStarting Address: ";
		if (current->starting_address == 0)
			cout << "0x0" <<endl;
		else{
			cout <<hex << showbase  << (current->starting_address) << endl;
		}
		i++;
		current = current->right;
	}
	cout << "\n------------------------------\n------------------------------\n";
}
