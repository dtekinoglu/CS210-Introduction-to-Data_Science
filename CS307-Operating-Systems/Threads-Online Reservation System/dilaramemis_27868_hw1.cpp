
#include <iostream>
#include <pthread.h>
#include <cstdlib>

using namespace std;

int seats[2][50]; //creating 2D array for seats in the plane
int reserved = 0; // keeps track of number of seats reserved so far.
int turn = 0; // for busy waiting
pthread_t TravelAgency1;
pthread_t TravelAgency2;
bool flag = false; // in order to print out "no seats left" statement only once. (not for two threads).



void * reserve(void * threadID) {
    
    int * ID = (int *) threadID;
    if (*ID ==1){

        while (true){
            while (turn != 0){}
            if(!flag)
			cout << "Agency 1 Entered Critical Region" << endl;  // If this is the first treat that reached full plane     
            if (reserved ==100){ //If all seats are reserved, terminate Agency 1 thread.
				if (flag == false) { // If this is the first treat that reached full plane
				cout << "Agency 1 Exit Critical Region"<< endl;
				cout << "No Seats Left" << endl << endl; 
				flag = true;
				}
				turn = 1; // turn must go back to other thread for it to be terminated.
                pthread_exit(NULL);

            }
        if (reserved !=100) {

            int num = rand() %100 +1; // generating random seat number.
            int row = 0;
            if (num >50) {
                row = 1;
                num -=50;
            }
            if (seats[row][num-1] == 0) { // if seat is not booked yet, reserve it.
                seats[row][num-1] = *ID;
                cout << "Seat Number " << num*(row+1) << " is reserved by Agency 1"<< endl;
                reserved +=1;
            }
			cout << "Agency 1 Exit Critical Region"<< endl << endl;
            turn = 1; // changes turn so that Agency 2 thread can run.        
        }
    }
    }
    
    else {
        
        while (true){
            while (turn != 1){}
            if(!flag) // If this is the first treat that reached full plane

			cout << "Agency 2 Entered Critical Region" << endl;

            if (reserved ==100){ //If all seats are reserved, terminate Agency 2 thread.
				if (flag == false) { // If this is the first treat that reached full plane
				cout << "Agency 2 Exit Critical Region"<< endl;
				cout << "No Seats Left" << endl << endl; 
				flag = true;
				}
				turn = 0; // turn must go back to other thread for it to be terminated.
                pthread_exit(NULL);

            }
        if (reserved !=100) {
            int num = rand() %100 +1; // generating random seat number.

            int row = 0;
            if (num >50) {
                row = 1;
                num -=50;
            }

            if (seats[row][num-1] == 0) {  // if seat is not booked yet, reserve it.
                seats[row][num-1] = *ID;
                cout << "Seat Number " << num*(row+1) << " is reserved by Agency 2"<< endl;
                reserved +=1;
            }
			cout << "Agency 2 Exit Critical Region"<< endl << endl;

            turn = 0; // changes turn so that Agency 2 thread can run.
        }
    }
        
}
}


int main()
{
    for (int i= 0; i <2; i++)  { // all seats are full with 0 in the beginning.
    for (int j = 0; j<50; j++) {
        seats[i][j] = 0;
    }
}

    srand(time(NULL));

    int id1 = 1;
    int id2 = 2;
    
    pthread_create(&TravelAgency1, NULL, reserve, (void*)&id1); //creating Agency 1
    pthread_create(&TravelAgency2, NULL, reserve, (void*)&id2); //creating Agency 2
    
    pthread_join(TravelAgency1, NULL);
    pthread_join(TravelAgency2, NULL);

	// After Agency 1 and Agency 2 are terminated, below code will run and show the seats of the plane.
    
    cout << "Plane is full: " << endl << endl;
    for (int x = 0; x < 2; x++){
        for (int j = 0; j < 50; j++){
            cout << seats[x][j];
        }
        cout << endl;
    }


    return 0;
}
