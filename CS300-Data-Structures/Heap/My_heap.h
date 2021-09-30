#ifndef MY_HEAP_H
#define MY_HEAP_H
#include<iostream>
#define MAX_CAPACITY 512


using namespace std;

struct memory_block{
	memory_block * right; //node to the right
	memory_block * left; //node to the left
	bool used; //if this memory block is reserved or not
	int size; //number of bytes reserved in Img_heap
	int starting_address; //the starting address in Img_heap of the memory reserved by this block

};

class My_heap{
private:
    memory_block * blk; // a pointer at the last memory block in my heap
    memory_block* heap_begin; // a pointer at the first memory block in my heap
    int used_bytes; // total number of allocated bytes in Img_memory
	int usedBlocks;
	int unusedBlocks;
	int leak;
public:
    My_heap();
    ~My_heap();
    memory_block* bump_allocate(int num_bytes);
    memory_block* first_fit_allocate(int num_bytes);
    memory_block* best_fit_allocate(int num_bytes);
    memory_block* first_fit_split_allocate(int num_bytes);
    void deallocate(memory_block* block_address);
    float get_fragmentation();
    void print_heap();
	string occupation(memory_block * block);
};

#endif