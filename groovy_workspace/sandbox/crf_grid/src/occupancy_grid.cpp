#ifndef OCCUPANCYGRID_H
#define OCCUPANCYGRID_H
#include "occupancy_grid.h"
#define DEBUG  false

occupancy_grid::occupancy_grid() {
	this->gridSize.push_back(10);
	this->constructGrid();
	this->iterationCount = 1;
	this->externalEnvironment = new Environment();

	this->linkPotentials.push_back(2); //potential for occupied
	this->linkPotentials.push_back(2); //potential for non occupied

	this->hiddenPotentials.push_back(0.5); //potential for occupied
	this->hiddenPotentials.push_back(0.5); //potential for non occupied
};

occupancy_grid::occupancy_grid(Environment * environment, int iterationCount) {
	vector<int> gridSize;

	gridSize.push_back(environment->getGridSizeHorizontal());
	gridSize.push_back(environment->getGridSizeVertical());	
	this->gridSize = gridSize;	

	this->dimensions = 2;	
//	this->constructGrid();
	this->iterationCount = iterationCount;
	this->externalEnvironment = environment;


	this->processedEnvironment = new Environment(environment);

	this->linkPotentials.push_back(10);
	this->linkPotentials.push_back(10);

	this->hiddenPotentials.push_back(0.5); //potential for occupied
	this->hiddenPotentials.push_back(0.5); //potential for non occupied
};

occupancy_grid::occupancy_grid(int dimensions, vector<int> gridSize, int iterationCount) {
	this->dimensions = dimensions;
	this->gridSize = gridSize;
	this->constructGrid();
	this->iterationCount = iterationCount;
	this->externalEnvironment = new Environment();
};

vector<vector<int>> occupancy_grid::getNeighbours(vector<int> currentCoords) {
	unsigned int currentCoord = 0;
//	vector<int>::iterator coordsIt;
	vector<vector<int>> neighbours;
	vector<int> currentNeighbour;
//	for (coordsIt = currentCoords.begin(); coordsIt != currentCoords.end(); coordsIt ++) {
	for (currentCoord = 0; currentCoord < currentCoords.size(); currentCoord ++) {
		if (currentCoords[currentCoord] > 0) {
			currentNeighbour = currentCoords;
			currentNeighbour[currentCoord] -= 1;
			neighbours.push_back(currentNeighbour);	
		}
		if (currentCoords[currentCoord] < this->gridSize[currentCoord] - 1) {
			currentNeighbour = currentCoords;
			currentNeighbour[currentCoord] += 1;
			neighbours.push_back(currentNeighbour);	
		}
	}
	return neighbours;
};

node * occupancy_grid::getNode(int x, int y) { //gets node in a 2d grid
	vector<int> coordinates;
	coordinates.push_back(x);
	coordinates.push_back(y);
	return this->grid.at(coordinates);
};

node * occupancy_grid::getNode(vector<int> coordinates) {
	return this->grid[coordinates];
};

void occupancy_grid::setIterationCount(int iterationCount) {
	this->iterationCount = iterationCount;
};

Environment * occupancy_grid::getProcessedEnvironment() {
	return this->processedEnvironment;
};

void occupancy_grid::constructGrid() {
	int dimension;
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
	If an environment was passed into the constructor then it will populate this->processedEnvironment and take value readings off this->externalEnvironment
*/
void occupancy_grid::loopyBeliefPropagation() {
	int iteration;
	float factorNodeMessage;//, currentNodeValue;	
//	node * currentNode;
//	vector<node *> processed;
//	queue<node *> unprocessed;
//	vector<node *>::iterator currentNeighbour, processedIt;

	vector<int> currentNode;
	vector<vector<int>> processed;
	queue<vector<int>> unprocessed;
	vector<vector<int>> neighbours;
	vector<vector<int>>::iterator currentNeighbour, processedIt;
	ROS_INFO("starting LBP...");	

	this->processedEnvironment->setMap(this->externalEnvironment->getMap());

	//set node (0, 0) as root
	unprocessed.push(vector<int>(2, 0));
	processed.push_back(vector<int>(2, 0));

	for (iteration = 0; iteration < this->iterationCount; iteration ++) {

		if (DEBUG) {
			cout << endl << "--new iteration--" << endl;
		}	

		while (!unprocessed.empty()) {
			//breadth first traversal order of grid
			currentNode = unprocessed.front();
			unprocessed.pop();

			//incoming messages are the sum-product of the neighbouring nodes
			factorNodeMessage = this->calculateIncomingMessages(currentNode);
			
			/*
			if (DEBUG) {
				cout << "current node : ";	
				cout << currentNode->getCoordinates()[0] << ", " << currentNode->getCoordinates()[1] << " - "; 
				cout << " value : " << factorNodeMessage;
				cout << endl;
			}
			*/
			//currentNode->setValue(factorNodeMessage);
//			currentNodeValue = this->processedEnvironment->getMapping(currentNode[0], currentNode[1]);
//			if (!(currentNodeValue > 0.5 + this->processedEnvironment->getOccupancyValueThreshold() 
//				|| currentNodeValue < 0.5 - this->processedEnvironment->getOccupancyValueThreshold())
//				) {
			this->processedEnvironment->setMapping(currentNode[0], currentNode[1], factorNodeMessage);
//			}
			//currentNode->setTempValue(factorNodeMessage);

			//add neighbour nodes to be processed
			neighbours = this->getNeighbours(currentNode);
			for (currentNeighbour = neighbours.begin(); currentNeighbour != neighbours.end(); currentNeighbour ++) { //loop through all neighbours
				processedIt = find(processed.begin(), processed.end(), * currentNeighbour);
				if (processedIt == processed.end()) {
					unprocessed.push(* currentNeighbour);
					processed.push_back(* currentNeighbour);
				}
			}				
		}
		//last node processed becomes the first node in the new iteration
		unprocessed.push(currentNode);
		processed.clear();
		processed.push_back(currentNode);
		//update changes throughout grid
		//map<vector<int>, node*>::iterator updater;
		//for (updater = this->grid.begin(); updater != this->grid.end(); updater ++) {
		//	updater->second->pushTempValue();
		//}
//		cout << "-----------" << endl;
		this->processedEnvironment->printMap();
	}
};

float occupancy_grid::calculateIncomingMessages(vector<int> currentNode) {
	vector<vector<int>>::iterator currentNeighbour;
	vector<vector<int>> neighbours;
	float nodeValue = this->processedEnvironment->getMapping(currentNode[0], currentNode[1]);
	float observedNodeValue = this->externalEnvironment->getMapping(currentNode[0], currentNode[1]);
	float incomingMessage = 0;
	float occupiedSum = nodeValue, nonOccupiedSum = (1 - nodeValue);

	neighbours = this->getNeighbours(currentNode);
	for (currentNeighbour = neighbours.begin(); currentNeighbour != neighbours.end(); currentNeighbour ++) { //loop through all neighbours
		//product sum incoming messages

		incomingMessage = this->processedEnvironment->getMapping((* currentNeighbour)[0], (* currentNeighbour)[1]);
		occupiedSum += this->hiddenPotentials[0] * incomingMessage;
		nonOccupiedSum += this->hiddenPotentials[1] * (1 - incomingMessage);
	}	
	
	occupiedSum += observedNodeValue * this->linkPotentials[0];
	nonOccupiedSum += (1 - observedNodeValue) * this->linkPotentials[1];

	return (1 / (occupiedSum + nonOccupiedSum)) * occupiedSum;
};

float occupancy_grid::calculateIncomingMessages(node * currentNode) {
	vector<node *>::iterator currentNeighbour;
	float nodeValue;

	nodeValue = this->processedEnvironment->getMapping(currentNode->getCoordinates()[0], currentNode->getCoordinates()[1]);

	float occupiedSum = nodeValue, nonOccupiedSum = 1 - occupiedSum;
	float factorValue = 0.5, incomingMessage = 0;

	for (currentNeighbour = currentNode->getNeighbours()->begin(); currentNeighbour != currentNode->getNeighbours()->end(); currentNeighbour ++) { //loop through all neighbours
		//product sum incoming messages

		incomingMessage = this->processedEnvironment->getMapping((* currentNeighbour)->getCoordinates()[0], (* currentNeighbour)->getCoordinates()[1]);
		occupiedSum += factorValue * incomingMessage;
		nonOccupiedSum += factorValue * (1 - incomingMessage);
	}	

	return (1 / (occupiedSum + nonOccupiedSum)) * occupiedSum;
};

void occupancy_grid::printGrid() {
	int x, y;
	for (y = 0; y < this->gridSize[1]; y ++) {
		for (x = 0; x < this->gridSize[0]; x ++) {
			cout.precision(2);
			cout << floor(this->getNode(x, y)->getValue() * 100) / 100 << "\t";
		}
		cout << endl << endl;
	}
};
#endif
