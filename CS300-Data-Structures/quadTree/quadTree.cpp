#include <iostream>
#include <string>
#include "quadTree.h"
#include <vector>

using namespace std;

vector<Node*> visited;
vector<Node*> cityList;



// Constructor
quadTree::quadTree(int x, int y){
	root = nullptr;
	grid_xcoord = x;
	grid_ycoord = y;

}

// Destructor
quadTree::~quadTree(void){
	makeEmpty();
}


// public method for making tree empty.
void quadTree::makeEmpty(){
	makeEmpty(root); //calling private method for make empty.

}


// private method for make tree rooted at t empty.
void quadTree::makeEmpty(Node * & t) const{
	if (t!=nullptr) {
		makeEmpty(t->northEast);
		makeEmpty(t->northWest);
		makeEmpty(t->southEast);
		makeEmpty(t->southWest);
		delete t;
	}
	t = nullptr;
}

// public method for printing the tree.
void quadTree::printTree(){
	pretty_print(root); // calling private method for printing the tree.
}


// private method for printing tree rooted at t.
void quadTree::pretty_print(Node* & t) const {
	if (t != nullptr){ // if the tree is not empty
		cout << t->name << endl;
		pretty_print(t->southEast); // recursively print the south east subtree
		pretty_print(t->southWest); // recursively print the south west subtree
		pretty_print(t->northEast); // recursively print the north east subtree
		pretty_print(t->northWest); // recursively print the north west subtree
	
	} 
}
// checking if the tree is empty.
bool quadTree::isEmpty() const{
	if (root == nullptr) return true;
	else return false;
}

// inserting new city to the quadtree
void quadTree::insertCity(string name, int x, int y) {
	insertCity(name, x, y, root);

}

void quadTree::insertCity(string newCity,int x, int y, Node * &t) { //ZATEN GRID'DE VARSA KISMINI KONTROL ETMEDÝM:
	if ( x <= grid_xcoord && y<= grid_ycoord){
	if (root == nullptr){
		root = new Node(newCity, x,y, nullptr, nullptr, nullptr, nullptr);
	}
	else{
		if (t == nullptr) 
			t = new Node(newCity, x,y, nullptr, nullptr, nullptr, nullptr);
		else if ( x >= t->x_coord && y>= t->y_coord) //BU ve devamýna >=, <= ekledim ama kontrol lazým.
			insertCity(newCity, x,y, t->northEast);
		else if ( x >= t->x_coord && y< t->y_coord) 
			insertCity(newCity, x,y, t->southEast);
		else if ( x < t->x_coord && y<= t->y_coord) 
			insertCity(newCity, x,y, t->southWest);
		else if ( x < t->x_coord && y> t->y_coord) 
			insertCity(newCity, x,y, t->northWest);
		else;
	}
	}
	else;
}

void quadTree::findCities(int x, int y, int r) {// fonksiyon imzasýndan const'u kaldýrdým. Kontrol et.
	visited.clear();
	cityList.clear();
	findCities(x,y,r, root);
	if (cityList.size() == 0) cout << "<None>";
	for (int i = 0; i< cityList.size();i++) {
		cout << cityList[i]->name;
		if (i != cityList.size()-1)
			cout << ", " ; 
	}
	cout << endl;
	for (int i = 0; i< visited.size();i++) {
		cout << visited[i]->name;
		if (i != visited.size()-1)
			cout << ", " ; 
	}
	cout << endl;
}

void quadTree::findCities(int x, int y, int r, Node * t) {

	if (t==nullptr) return ;
	visited.push_back(t);
	double dist = (t->x_coord-x)*(t->x_coord-x)+ (t->y_coord-y)*(t->y_coord-y);
	double distance = sqrt(dist);

	if (t->x_coord >= x-r && t->x_coord <= x+r && t->y_coord >= y-r && t->y_coord <= y+r) {// Inside the square.
		if (distance <= r) {
			cityList.push_back(t);
			findCities(x,y,r, t->southEast);
			findCities(x,y,r, t->southWest);
			findCities(x,y,r, t->northEast);
			findCities(x,y,r, t->northWest);
			
		}
		else{
		if(t->x_coord <= x && t->y_coord <= y){ //region 11
			findCities(x,y,r, t->southEast);
			findCities(x,y,r, t->northEast);
			findCities(x,y,r, t->northWest);

		}
		else if(t->x_coord >= x && t->y_coord >= y){ //region 10
			findCities(x,y,r,t->southEast);
			findCities(x,y,r, t->southWest);
			findCities(x,y,r, t->northWest);

		}
		else if(t->x_coord >= x && t->y_coord <= y){ //region 12
			findCities(x,y,r, t->southWest);
			findCities(x,y,r, t->northEast);
			findCities(x,y,r, t->northWest);

		}
		else if(t->x_coord <= x && t->y_coord >= y){ //region 9
			findCities(x,y,r,t->southEast);
			findCities(x,y,r, t->southWest);
			findCities(x,y,r, t->northEast);			
		}
	}
	}
	else if (t->x_coord <= x-r && t->y_coord >= y-r && t->y_coord <= y+r){ // region 4
		findCities(x,y,r, t->southEast);
		findCities(x,y,r, t->northEast);



	}
	else if (t->x_coord >= x+r && t->y_coord >= y-r && t->y_coord <= y+r) { // region 5
		findCities(x,y,r, t->southWest);
		findCities(x,y,r, t->northWest);

	}
	else if (t->x_coord >= x-r && t->x_coord <= x+r && t->y_coord <= y-r){ //region 7
		findCities(x,y,r, t->northEast);
		findCities(x,y,r, t->northWest);

	}
	else if  (t->x_coord >= x-r && t->x_coord <= x+r && t->y_coord >= y+r){//region 2
		findCities(x,y,r, t->southEast);
		findCities(x,y,r, t->southWest);
	}
	else if (t->x_coord <=x-r && t->y_coord <= y-r){ // R in region 6.
		findCities(x,y,r,t->northEast);
	}
	else if (t->x_coord >= x+r && t->y_coord <= y-r){ // R in region 8.
		findCities(x,y,r, t->northWest);
	}
	else if (t->x_coord <= x-r && t->y_coord >= y+r) {// R in region 1.
		findCities(x,y,r,t->southEast);
	}
	else if (t->x_coord >= x+r && t->y_coord >= y+r) {// R in region 3
		findCities(x,y,r, t->southWest);
	}
	else return;
}



