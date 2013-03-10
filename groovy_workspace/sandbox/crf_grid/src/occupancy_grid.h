#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <math.h>
#include <iostream>
#include <iterator>
#include <queue>
#include "Environment.cpp"
#include "node.cpp"

class occupancy_grid {
protected:
	int	dimensions; //default dimension is 2;
	vector<int> gridSize; //grid size, indexed by dimensions
	map<vector<int>, node*> grid; //contains the grid, vectors of the coodinates mapping to the node
	int iterationCount; //number of iterations for loopy belief propagation
	Environment * externalEnvironment; //environment passed in to be processed
	Environment * processedEnvironment; //copy of the original environment which has been processed

public:
	occupancy_grid();
	occupancy_grid(int dimensions, vector<int> gridSize, int iterationCount);
	occupancy_grid(Environment * environment, int iterationCount);
	void constructGrid(); //contructs or reconstructs the grid according the number of dimensions passed	
	void loopyBeliefPropagation(); //performs LBP
	void printGrid(); //used for debugging in a 2d grid, prints out the grid on command line
	node * getNode(int x, int y); //gets node in a 2d grid
	node * getNode(vector<int> coordinates);
	void setIterationCount(int iterationCount);
	Environment * getProcessedEnvironment();

private:
	void __constructGrid(node * currentNode); //recusive function to iterate through all coordinates
	float calculateIncomingMessages(node * currentNode); //calculates the product of all incoming messages from all function node to variable node
};
