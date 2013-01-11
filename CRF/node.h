#include <vector>
using namespace std;

class node {
protected:
	vector<node*> *neighbours;
	vector<int> coordinates; //coordinates of the node in n-dimensions
	float nodeValue; //0.5 indicates unknown occupancy, -1 is used for edges (non-existant neighbours)

public:
	node () {
		nodeValue = 0.5;
		neighbours = new vector<node *>();
	}
	node(int dimensions) {
		nodeValue = 0.5;
		neighbours = new vector<node*>(2 * dimensions);
	};

	vector<node*> getNeighbours() {return *this->neighbours;};
	void addNeighbour(node * neighbour) {this->neighbours->push_back(neighbour);};
	float getValue() {return this->nodeValue;};
	void setValue(float value) {this->nodeValue = value;};
};