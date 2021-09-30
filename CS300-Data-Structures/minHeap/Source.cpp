#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <functional>
#include "minHeap.h"

using namespace std;
vector<string> rowList;
vector<string> blockList;
int b,c,r;
vector<vector<vector<string>>> stadium;
unordered_map<string, tuple<int,int,int>>  customers;
unordered_map<string,int> blockPositions;
unordered_map<string,int> rowPositions;
vector<minHeap> heapList;
ofstream out;



//checks the hash map to see if the customer name is already exists
bool checkCustomer(string name){
	if (customers.find(name) != customers.end()) return true;
	return false;
}


//prints out the whole stadium block by block
void print(){
	out <<endl;
	int temp = 1;
	for(temp; temp <b; temp++){
		out << blockList[temp] <<  endl;
		out << "~~~~~~~"<<endl;
		for(int j = 0; j <r; j++){
			out << rowList[rowPositions[rowList[j]]] << " : ";
			for(int k = 0; k <c; k++){
				out << stadium[temp][j][k] << " ";
			}
			out << endl;
		}
		out<<"======="<<endl<<endl;
	}
}


//reservation by the empties block. 
void reserveByRow(int row, string name){
	if (customers.find(name) != customers.end())  out <<name << " could not reserve a seat!" << endl; // if there is already a reservation with the given name.
	else{
		int block = heapList[row].reserveSeatByRow(); //gets the empties block ID.

	bool flag = false;
	int cnum;
	for (int i = 0; i < c; i++){
		if (stadium[block][row][i] == "---"){
			stadium[block][row][i] = name.substr(0,3);
			flag = true;
			cnum = i;
			break;
		}

	}
	if (flag ){ //if there is any empty column.
		tuple<int,int,int> ex = make_tuple(block, row, cnum);
		customers[name] = ex;
		out << name << " has reserved "<< blockList[block] << " " << rowList[row] << "-" <<cnum  << " by emptiest block" << endl;


	}
	else{
		out <<name << " could not reserve a seat!" << endl;
	}
	}
}


//reservation by full seat info.
void reserveByFullInfo(int block, int row, int c, string name){
	if (stadium[block][row][c] !="---" || customers.find(name) != customers.end())  out <<name << " could not reserve a seat!" << endl;
	else{
		stadium[block][row][c] = name.substr(0,3);
		tuple<int,int,int> ex = make_tuple(block, row, c);
		customers[name] = ex;
		out << name << " has reserved "<< blockList[block] << " " << rowList[row] << "-" <<c  << endl;
		heapList[row].reserveByFull(block); //updates heap accordingly
	}

}

//finds the seat of a customer if there is any.
bool getSeat(string name){
	if (customers.find(name) != customers.end()){
		out <<"Found that " << name << " has a reservation in " << blockList[get<0>(customers[name])] << " " <<rowList[get<1>(customers[name])] <<"-"<<get<2>(customers[name])
			<< endl;
		return true;
	}
	else{
		out << "There is no reservation made for " << name <<"!"<< endl;
		return false;
	}

}


//cancel the reservation of a customer if there is any.
void cancelReservation(string name){
	if (checkCustomer(name)){
		int block = get<0>(customers[name]);
		int row_ = get<1>(customers[name]);
		int col_ = get<2>(customers[name]);
		stadium[block][row_][col_] = "---";
		heapList[row_].cancel(block);
		customers.erase(name);
		out << "Cancelled the reservation of " << name << endl;
	}
	else{
		out << "Could not cancel the reservation for " << name <<"; no reservation found!" << endl;
	}



}

int main(){


	// open the input file and output file.
	string filename = "inputs.txt";
	string outFile = "output.txt";
	ifstream in;
	in.open(filename.c_str());
	out.open(outFile.c_str());

	//dummy head for block list.(to make it consistent with heap class)
	blockList.push_back("");


	//getting block names into blockList vector.
	string line, blockName;
	getline(in, line);
	istringstream parse(line);
	while (parse >> blockName) blockList.push_back(blockName); //for (int i = 0; i < blockList.size(); i++) cout << blockList[i] << " ";

	//getting row names into rowList vector.
	getline(in,line);
	string rowName;
	istringstream _parse(line);
	while (_parse >> rowName) rowList.push_back(rowName); //for (int i = 0; i < rowList.size(); i++) cout << rowList[i] << " ";


	//getting number of columns
	getline(in, line); 
	istringstream __parse(line); __parse>> c; //cout << colNum << endl;

	b = blockList.size();
	r = rowList.size();


	//mapping positions of blocks and rows to the hash table.
	for(int i = 0; i < b; i++) blockPositions[blockList[i]] = i;
	for(int i = 0; i < r; i++) rowPositions[rowList[i]] = i;

	

	//initializing stadium
	vector<string>tempRow(c,"---");
	vector<vector<string>> tempBlock(r,tempRow);
	for(int i = 0; i <b; i++) stadium.push_back(tempBlock);


	//creating a minHeap for each row.
	for (int i = 0; i < r; i++){
		minHeap temp;
		heapList.push_back(temp);
	}

	string command,name,bname,rname;
	int cnum;
	while (in.eof()==false){
		getline(in,line);
		if (line !=""){
		istringstream parse(line);
		parse>>command;

		if (command == "reserve_seat") {
			parse>>name>>bname>> rname>>cnum;
			reserveByFullInfo(blockPositions[bname], rowPositions[rname], cnum, name);
		}
		else if(command == "get_seat") {
			parse>>name;
			getSeat(name);
		}
		else if (command == "print") {
			print();
		}
		else if (command == "cancel_reservation") {
			parse>>name;
			cancelReservation(name);
		}
		else if(command =="reserve_seat_by_row"){
			parse>>name>> rname;
			reserveByRow(rowPositions[rname],name);


			
		}
		}
	}
	out.close();
	in.close();


	return 0;
}

