#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <map>
#include "node.h"
using namespace std;

class occupancy_grid {

protected:
	int dimensions; //default dimension is 2;
	vector<int> gridSize; //grid size, indexed by dimensions
	map<vector<int>, node*> grid; //contains the grid, vectors of the coodinates mapping to the node

public:
	occupancy_grid() {
		this->gridSize.push_back(10);
		this->constructGrid();
	};
	occupancy_grid(int dimensions, vector<int> gridSize) {
		this->dimensions = dimensions;
		this->gridSize = gridSize;
		this->constructGrid();
	};
	void constructGrid(); //contructs or reconstructs the grid according the number of dimensions passed
	
	void generaliseMap(map<pair<int, int>, float> mapping); //used to convert the format from the sensor simulator
	void loopyBeliefPropagation(map<vector<int>, node*> grid); //performs LBP and updates the grid
	void printGrid(); //used for debugging in a 2d grid, prints out the grid on command line
	node * getNode(int x, int y) { //gets node in a 2d grid
		vector<int> coordinates;
		coordinates.push_back(x);
		coordinates.push_back(y);
		return this->grid.at(coordinates);
	};
	node * getNode(vector<int> coordinates) {return this->grid[coordinates];};

private:
	void __constructGrid(node * currentNode);
};