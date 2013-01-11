#include "occupancy_grid.h"
using namespace std;


void occupancy_grid::constructGrid(int dimensions) {
	int dimension, dimIndex, i, j;
	vector<int> currentNode, defaultNode;
	//populate the grid with nodes
	
	//initialise default vector with 0's
	for (dimension = 0; dimension < dimensions; dimension ++) {
		defaultNode.push_back(0);
	}

	for (dimension = 0; dimension < dimensions; dimension ++) {
		for (dimIndex = 0; dimIndex < this->gridSize[dimension]; dimIndex ++) {
			for (i = 0; i < dimensions; i ++) {
				for (j = 0; j < this->gridSize[i]; j ++) {

			}
		}
	}

	//connect each node with its neighbours
};


void occupancy_grid::loopyBeliefPropagation(map<vector<int>, node> grid) {
	
};

void occupancy_grid::printGrid() {
	int x, y;
	for (y = 0; y < this->gridSize[1]; y ++) {
		for (x = 0; x < this->gridSize[0]; x ++) {
			cout << this->getNode(x, y).getValue() << "\t";
		}
		cout << endl << endl;
	}
};
