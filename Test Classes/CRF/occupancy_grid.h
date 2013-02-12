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
	int iterationCount; //number of iterations for loopy belief propagation

public:
	occupancy_grid() {
		this->gridSize.push_back(10);
		this->constructGrid();
		this->iterationCount = 1;
	};
	occupancy_grid(int dimensions, vector<int> gridSize, int iterationCount) {
		this->dimensions = dimensions;
		this->gridSize = gridSize;
		this->constructGrid();
		this->iterationCount = iterationCount;
	};
	void	constructGrid(); //contructs or reconstructs the grid according the number of dimensions passed	
	void	generaliseMap(map<pair<int, int>, float> mapping); //used to convert the format from the sensor simulator
	void	loopyBeliefPropagation(); //performs LBP
	void	printGrid(); //used for debugging in a 2d grid, prints out the grid on command line
	node * getNode(int x, int y) { //gets node in a 2d grid
		vector<int> coordinates;
		coordinates.push_back(x);
		coordinates.push_back(y);
		return this->grid.at(coordinates);
	};
	node * getNode(vector<int> coordinates) {return this->grid[coordinates];};
	void	setIterationCount(int iterationCount);
private:
	void	__constructGrid(node * currentNode); //recusive function to iterate through all coordinates
	float	calculateIncomingMessages(node * currentNode); //calculates the product of all incoming messages from all function node to variable node
	float	factorFunction(float currentvalue, float incomingValue);
	void	normaliseGrid();
};