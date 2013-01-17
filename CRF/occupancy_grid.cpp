#include "occupancy_grid.h"
#include <time.h>

//debug
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
#define DEBUG false

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
	
/*
	Performs loopy belief propagation, uses abstract function nodes representing the edges between each node.
	Iterates through the grid occupancy_grid::iterationCount number of times
*/
void occupancy_grid::loopyBeliefPropagation() {
	map<vector<int>, node *> randomNodeGrid;
	int pickRandomNode, iteration;
	float product = 1, sum;
	float factorNodeMessage;
	map<vector<int>, node*>::iterator currentNode;	
	srand(time(NULL));

	for (iteration = 0; iteration < this->iterationCount; iteration ++) {
		randomNodeGrid = grid;
		if (DEBUG) {
			cout << endl << "--new iteration--" << endl;
		}
		while (randomNodeGrid.size() != 0) { //parse incoming message for every single node		
			//pick random node from grid
			pickRandomNode = rand() % (randomNodeGrid.size());
			currentNode = randomNodeGrid.begin();
			std::advance(currentNode, pickRandomNode);
		
			//incoming messages are the sum-product of the neighbouring nodes
			factorNodeMessage = this->calculateIncomingMessages(currentNode->second);		
			if (DEBUG) {
				cout << "current node : ";
				std::copy(currentNode->first.begin(),currentNode->first.end(), ostream_iterator<int>(cout, "-"));
				cout << " value : " << factorNodeMessage;
				cout << endl;
			}
			this->grid[currentNode->first]->setValue(factorNodeMessage);
			randomNodeGrid.erase(currentNode);
		}
	}

	//this->normaliseGrid();
};

float occupancy_grid::calculateIncomingMessages(node * currentNode) {
	float product = 1;
	vector<node *>::iterator currentNeighbour;
	
	for (currentNeighbour = currentNode->getNeighbours()->begin(); currentNeighbour != currentNode->getNeighbours()->end(); currentNeighbour ++) { //loop through all neighbours
		//product of all neighbour's messages
		product *= (* currentNeighbour)->getValue(), currentNode->getValue();
	}	

	return factorFunction(currentNode->getValue(), product);
};

void occupancy_grid::normaliseGrid() {
	map<vector<int>, node*>::iterator currentNode;
	float min = -HUGE_VAL, max = HUGE_VAL, currentValue;

	for (currentNode = this->grid.begin(); currentNode != this->grid.end(); currentNode ++) {
		currentValue = currentNode->second->getValue();
		if (currentValue < max) {
			max = currentValue;
		}
		if (currentValue > min) {
			min = currentValue;
		}
	}
	for (currentNode = this->grid.begin(); currentNode != this->grid.end(); currentNode ++) {
		currentNode->second->setValue((max - currentValue) / min);
	}
};
float occupancy_grid::factorFunction(float currentValue, float incomingValue) {
	float calculatedValue = (currentValue + incomingValue) / 2;

	return incomingValue;
	return calculatedValue;
};

void occupancy_grid::printGrid() {
	int x, y;
	for (y = 0; y < this->gridSize[1]; y ++) {
		for (x = 0; x < this->gridSize[0]; x ++) {
			cout.precision(2);
			cout << this->getNode(x, y)->getValue() << "\t";
		}
		cout << endl << endl;
	}
};

void occupancy_grid::setIterationCount(int iterationCount) {
	this->iterationCount = iterationCount;
};