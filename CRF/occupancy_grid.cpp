#include "occupancy_grid.h"
using namespace std;

void occupancy_grid::constructGrid() {
	int dimension, dimIndex, dimensionIterator;
	vector<int> newCoords, defaultCoords;
	node * newNode = new node();
	node * defaultNode = new node();
	//populate the grid with nodes
	
	//initialise default vector with 0's
	for (dimension = 0; dimension < this->dimensions; dimension ++) {
		defaultCoords.push_back(0);
	}

	defaultNode = new node(defaultCoords);
	this->grid[defaultCoords] = defaultNode;

	for (dimension = 0; dimension < this->dimensions; dimension ++) { //process each dimension
		//shift position of coordinates by 1 in each dimension and recusively process
		if (defaultCoords[dimension] + 1 < this->gridSize[dimension]) { //check for specific case
			newCoords = defaultCoords;
			newCoords[dimension] += 1;
			newNode = new node(newCoords);
			this->grid[newCoords] = newNode;

			//make both nodes neighbours
			defaultNode->addNeighbour(newNode);
			newNode->addNeighbour(defaultNode);

			__constructGrid(newNode);
		}
	}

};

void occupancy_grid::__constructGrid(node * currentNode) {
	int dimension;
	vector<int> currentCoords = currentNode->getCoords();
	vector<int> newCoords;
	node * newNode = new node();
	map<vector<int>, node*>::iterator it;
	
	for (dimension = 0; dimension < this->dimensions; dimension ++) { //process each dimension

		//shift position of coordinates by 1 in each dimension and recusively process
		if (currentCoords[dimension] + 1 < this->gridSize[dimension]) { //check for specific case
			newCoords = currentCoords;
			newCoords[dimension] += 1;
			it = this->grid.find(newCoords);

			if (this->grid.end() != it) { //checks if node has already been added to grid
				//already exists, but still add current node as neighbour, don't do anything else
				newNode = this->grid[newCoords];
				currentNode->addNeighbour(newNode);
				newNode->addNeighbour(currentNode);
			}
			else {
				newNode = new node(newCoords);
				this->grid[newCoords] = newNode;

				//make both nodes neighbours
				currentNode->addNeighbour(newNode);
				newNode->addNeighbour(currentNode);			

				__constructGrid(newNode);
			}
		}
	}
};
	
void occupancy_grid::loopyBeliefPropagation(map<vector<int>, node*> grid) {
	
};

void occupancy_grid::printGrid() {
	int x, y;
	for (y = 0; y < this->gridSize[1]; y ++) {
		for (x = 0; x < this->gridSize[0]; x ++) {
			cout << this->getNode(x, y)->getValue() << "\t";
		}
		cout << endl << endl;
	}
};
