#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
using namespace std;

int main(){
	string line;
	ifstream myFile("loremipsum.txt");
	int count = 0;
	double totalTime = 0.0;
	clock_t start = clock();
	while(getline(myFile, line)){
		for(int i = 0; i < line.length();i++){
			if (line[i] == 'a') count++;

	}
	}
	clock_t stop = clock();
	totalTime = (stop-start)/CLOCKS_PER_SEC;
	myFile.close();
	cout << "total time in seconds is " << totalTime << endl; 
	cout <<"The count of a's is " <<count << endl;



return 0;
}
