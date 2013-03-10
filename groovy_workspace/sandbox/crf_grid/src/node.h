#include <vector>

class node {
protected:
	std::vector<node *> *neighbours;
	std::vector<int> coordinates; //coordinates of the node in n-dimensions
	float nodeValue; //1 indicates unknown occupancy, 0 is for un-occupied, 2 is for occupied. Range of nodeValue is (0, 2)
	float __LBPTEMPVALUE; //temporary value holding the value of each node during iteration of loopy belief propagation
	bool fixed; //flag for whether the node has been observed, it's value is known hence it cannot be changed during LBP
public:
	node();
	node(std::vector<int> coordinates);
	std::vector<node *> * getNeighbours();
	void addNeighbour(node * neighbour);
	float getValue();
	bool setValue(float value); //returns false if the value is fixed, cannot be changed. Limits value between the range of 0 and 2.
	std::vector<int> getCoords();
	void setTempValue(float tempVal);
	bool pushTempValue(); //Overwrites nodeValue using __LBPTEMPVALUE at the end of iteration
	void setFixedValue(float value);
	std::vector<int> getCoordinates();

};

	