

#include <iostream>
#include <string>

using namespace std;

struct Node {
	string name;
	int x_coord;
	int y_coord;
	Node * northWest;
	Node * northEast;
	Node * southWest;
	Node * southEast;
	Node (string word, int x=0, int y=0, Node* nw = nullptr, Node* ne = nullptr, Node* sw = nullptr, Node* se = nullptr): 
		name(word), x_coord(x), y_coord(y), northWest(nw), northEast(ne), southWest(sw), southEast(se){}
};

class quadTree{

public:
	quadTree(int x, int y); // constructor
	~quadTree(void); // destructor
	void insertCity(string, int, int);
	bool isEmpty() const;
	void printTree();
	void findCities(int x, int y, int r);
	void makeEmpty();

private:
	Node * root;
	int grid_xcoord;
	int grid_ycoord;
	void remove( const Node & x, Node * & t ) const;
	void makeEmpty(Node * & t) const;
	void pretty_print(Node * & t) const;
	void insertCity(string newCity, int x, int y, Node * &t);
	void findCities(int x, int y, int r, Node * t) ;




};
