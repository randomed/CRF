#ifndef OCCUPANCYGRID_H
#define OCCUPANCYGRID_H
#include "occupancy_grid.h"
#define DEBUG  false
occupancy_grid::occupancy_grid() {
	/*
	this->gridSize.push_back(10);
	this->constructGrid();
	this->iterationCount = 1;
	this->externalEnvironment = new Environment();
//	int gridSizeHorizontal = this->externalEnvironment->getGridSizeHorizontal();
//	int gridSizeVertical = this->externalEnvironment->getGridSizeHorizontal();
	this->linkPotentials.push_back(10); //potential for occupied
	this->linkPotentials.push_back(10); //potential for non occupied

	this->hiddenPotentials.push_back(0.5); //potential for occupied
	this->hiddenPotentials.push_back(0.5); //potential for non occupied
	*/
	/*	
	this->__processedMessages1 = new float *[gridSizeHorizontal];  
	this->__processedMessages2 = new float *[gridSizeHorizontal]; 
 
	for(int i = 0; i< gridSizeHorizontal; i++) {
			this->__processedMessages1[i] = new float [gridSizeVertical];
			this->__processedMessages2[i] = new float [gridSizeVertical];
	}
	*/
};

occupancy_grid::occupancy_grid(Environment * environment, int iterationCount) {
	vector<int> gridSize;
//	int gridSizeHorizontal = environment->getGridSizeHorizontal();
//	int gridSizeVertical = environment->getGridSizeHorizontal();

	gridSize.push_back(environment->getGridSizeHorizontal()); //redundant remove later
	gridSize.push_back(environment->getGridSizeVertical());	
	this->gridSize = gridSize;	

	this->dimensions = 2;	
	this->constructGrid();
	this->iterationCount = iterationCount;
	this->externalEnvironment = environment;

	this->processedEnvironment = new Environment(environment);

	this->hiddenPotentials.push_back(10); //occupied - occupied
	this->hiddenPotentials.push_back(1); //occupied - non occupied
	this->hiddenPotentials.push_back(1); //non occupied - occupied
	this->hiddenPotentials.push_back(10); //non occupied - non occupied
	
	this->linkPotentials.push_back(1); //potential for occupied
	this->linkPotentials.push_back(1); //potential for non occupied

	/*
	this->__processedMessages1 = new float *[gridSizeHorizontal];  
	this->__processedMessages2 = new float *[gridSizeHorizontal]; 
 
	for(int i = 0; i< gridSizeHorizontal; i++) {
			this->__processedMessages1[i] = new float [gridSizeVertical];
			this->__processedMessages2[i] = new float [gridSizeVertical];
	}
	*/
};

occupancy_grid::occupancy_grid(int dimensions, vector<int> gridSize, int iterationCount) {//remove later
	this->dimensions = dimensions;
	this->gridSize = gridSize;
	this->constructGrid();
	this->iterationCount = iterationCount;
	this->externalEnvironment = new Environment();
};
/*
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
*/
/*
float occupancy_grid::getMessage(vector<int> neighbourNode, vector<int> currentNode) {
	if (neighbourNode[0] < currentNode[0] || neighbourNode[1] < currentNode[1]) {
		return this->__processedMessages2[neighbourNode[0]][neighbourNode[1]];
	}
	else {
	
		return this->__processedMessages1[currentNode[0]][currentNode[1]];
	}	
};

void occupancy_grid::setMessage(vector<int> neighbourNode, vector<int> currentNode, float message) {
	if (neighbourNode[0] < currentNode[0] || neighbourNode[1] < currentNode[1]) {
		this->__processedMessages2[neighbourNode[0]][neighbourNode[1]] = message;
	}
	else {
	
		this->__processedMessages1[currentNode[0]][currentNode[1]] = message;
	}	
};
*/

float occupancy_grid::normalise(float val1, float val2) {
	return (1 / (val1 + val2)) * val1;
};

void occupancy_grid::setHiddenPotentials(vector<float> potentials) {
	this->hiddenPotentials = potentials;
};
void occupancy_grid::setLinkPotentials(vector<float> potentials) {
	this->linkPotentials = potentials;
};

vector<float> occupancy_grid::getHiddenPotentials() {
	return this->hiddenPotentials;
};

vector<float> occupancy_grid::getLinkPotentials() {
	return this->linkPotentials;
};
void occupancy_grid::incrementHiddenPotential(float jump) {
	this->hiddenPotentials[0] += jump;
	this->hiddenPotentials[1] += jump;

//	this->linkPotentials[0] += jump; //temporary test to get ROC curves working
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
//			it = this->grid.find(newCoords);

//			if (this->grid.end() != it) { //checks if node has already been added to grid
			if (this->grid.count(newCoords) > 0) {
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
	node * currentNode;
	vector<node *> processed;
	std::vector<node*>::iterator processedIt;
	queue<node *> unprocessed;
	std::map<node *, vector<float>>::iterator currentNeighbour;
	std::map<node *, vector<float>> neighbours;
	std::ostringstream oss;
//	vector<int> currentNode;
//	vector<vector<int>> processed;
//	queue<vector<int>> unprocessed;
//	vector<vector<int>> neighbours;
//	vector<vector<int>>::iterator currentNeighbour, processedIt;

//	ROS_INFO("starting LBP...");	
//	int gridSizeHorizontal = this->externalEnvironment->getGridSizeHorizontal(), gridSizeVertical = this->externalEnvironment->getGridSizeVertical();	
	//reset messages
//	for(int i = 0; i < gridSizeHorizontal; i++) {
//			std::fill_n(this->__processedMessages1[i], gridSizeVertical, 0.5);
//			std::fill_n(this->__processedMessages2[i], gridSizeVertical, 0.5);
//	}
//	this->processedEnvironment->setMap(this->externalEnvironment->getMap());
	this->processedEnvironment->generateUnknownMap();

	//set node (0, 0) as root
//	unprocessed.push(vector<int>(2, 0));
	unprocessed.push(this->getNode(vector<int>(2, 0)));
//	processed.push_back(vector<int>(2, 0));
	processed.push_back(this->getNode(vector<int>(2, 0)));

	for (iteration = 0; iteration < this->iterationCount; iteration ++) {

		if (DEBUG) {
			cout << endl << "--new iteration--" << endl;
			
/*			cout << "contents of messages:" << endl;			
			for (int x = 0; x < this->externalEnvironment->getGridSizeHorizontal(); x++) {
					for (int y = 0; y < this->externalEnvironment->getGridSizeVertical(); y++) {
						cout << x << ", " << y << " == " << this->__processedMessages1[x][y] << endl;
					}
	
			} */
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
			currentNode->setValue(factorNodeMessage);
//			currentNodeValue = this->processedEnvironment->getMapping(currentNode[0], currentNode[1]);
//			if (!(currentNodeValue > 0.5 + this->processedEnvironment->getOccupancyValueThreshold() 
//				|| currentNodeValue < 0.5 - this->processedEnvironment->getOccupancyValueThreshold())
//				) {
			this->processedEnvironment->setMapping(currentNode->getCoords()[0], currentNode->getCoords()[1], factorNodeMessage);
//			}
			//currentNode->setTempValue(factorNodeMessage);
			
			//add neighbour nodes to be processed
//			neighbours = this->getNeighbours(currentNode);
			neighbours = currentNode->getNeighbours();
			
			for (currentNeighbour = neighbours.begin(); currentNeighbour != neighbours.end(); currentNeighbour ++) { //loop through all neighbours
//				processedIt = find(processed.begin(), processed.end(), * currentNeighbour);
				processedIt = std::find(processed.begin(), processed.end(), currentNeighbour->first);
				
				if (processedIt == processed.end()) {
//					unprocessed.push(* currentNeighbour);
//					processed.push_back(* currentNeighbour);
					unprocessed.push(currentNeighbour->first);
					processed.push_back(currentNeighbour->first);
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
//		this->processedEnvironment->printMap();
		oss.str("");
		oss << iteration + 3;
		oss << "iteration" << iteration + 1;
		this->processedEnvironment->writeToFile(oss.str());
	}
};
/*
float occupancy_grid::calculateIncomingMessages(vector<int> currentNode) {
	vector<vector<int>>::iterator currentNeighbour;
	vector<vector<int>> neighbours;
	vector<vector<int>>::iterator currentNeighboursNeighbour;
	vector<vector<int>> neighboursNeighbours;
	float nodeValue = this->processedEnvironment->getMapping(currentNode[0], currentNode[1]);
	float observedNodeValue = this->externalEnvironment->getMapping(currentNode[0], currentNode[1]);
	float neighbourOccupiedProduct = 1, neighbourNonoccupiedProduct = 1, neighbourIncomingMessage = 0, normalisedOccupiedProduct = 0;
	float occupiedSum = 0, nonOccupiedSum = 0, occupiedProduct = 1;
	int neighbourX, neighbourY;

	neighbours = this->getNeighbours(currentNode);
	for (currentNeighbour = neighbours.begin(); currentNeighbour != neighbours.end(); currentNeighbour ++) { //loop through all neighbours
		neighbourX = (* currentNeighbour)[0];
		neighbourY = (* currentNeighbour)[1];
		//product sum incoming messages
		//loop through neighbour's neighbours except current node
		neighboursNeighbours = this->getNeighbours(* currentNeighbour);
		for (currentNeighboursNeighbour = neighboursNeighbours.begin(); currentNeighboursNeighbour != neighboursNeighbours.end(); currentNeighboursNeighbour ++) {
			if (currentNode != * currentNeighboursNeighbour) {
				neighbourIncomingMessage = this->getMessage(* currentNeighboursNeighbour, * currentNeighbour);
				neighbourOccupiedProduct *= this->hiddenPotentials[0] * neighbourIncomingMessage;
				neighbourNonoccupiedProduct *= this->hiddenPotentials[1] * (1 - neighbourIncomingMessage);
			}
		}
		//factor in current beliefs, hidden potential
		neighbourOccupiedProduct *= this->hiddenPotentials[0] * this->processedEnvironment->getMapping(neighbourX, neighbourY);
		neighbourNonoccupiedProduct *= this->hiddenPotentials[1] * (1 - this->processedEnvironment->getMapping(neighbourX, neighbourY));
		//factor in observations and link potential
		neighbourOccupiedProduct *= this->linkPotentials[0] * this->externalEnvironment->getMapping(neighbourX, neighbourY);
		neighbourNonoccupiedProduct *= this->linkPotentials[1] * (1 - this->externalEnvironment->getMapping(neighbourX, neighbourY));

		normalisedOccupiedProduct = this->normalise(neighbourOccupiedProduct, neighbourNonoccupiedProduct);
		if (DEBUG) {
			cout << (*currentNeighbour)[0] << ", " << (*currentNeighbour)[1] << " -> " << currentNode[0] << ", " << currentNode[1] << " = " << normalisedOccupiedProduct 
			<< " -- occupied product: " << neighbourOccupiedProduct << " nonoccupied product: " << neighbourNonoccupiedProduct << endl;
		}
		this->setMessage(*currentNeighbour, currentNode, normalisedOccupiedProduct);	
				
//		incomingMessage = this->processedEnvironment->getMapping((* currentNeighbour)[0], (* currentNeighbour)[1]);
//		occupiedSum += this->hiddenPotentials[0] * incomingMessage;
//		nonOccupiedSum += this->hiddenPotentials[1] * (1 - incomingMessage);
		occupiedSum += neighbourOccupiedProduct;
		nonOccupiedSum += neighbourNonoccupiedProduct;
		neighbourOccupiedProduct = 1;
		neighbourNonoccupiedProduct = 1;
	}	
	
	occupiedSum += nodeValue;
	occupiedSum += observedNodeValue * this->linkPotentials[0];
	nonOccupiedSum += (1 - nodeValue);
	nonOccupiedSum += (1 - observedNodeValue) * this->linkPotentials[1];
	
	if (DEBUG) {
		//cout << "incoming message printout" << endl;
		cout << currentNode[0] << ", " << currentNode[1] << " == " << nodeValue << ", " << observedNodeValue << endl;
	}	
	return normalise(occupiedSum, nonOccupiedSum);
//	return (1 / (occupiedSum + nonOccupiedSum)) * occupiedSum;
};
*/

float occupancy_grid::calculateIncomingMessages(node * currentNode) {
	std::map<node *, vector<float>> n, nn; //neighbours, neighbour's neighbour
	std::map<node *, vector<float>>::iterator currentn, currentnn;
	int variablesTracked = currentNode->getVariablesTracked();
	vector<float> defaultVector = vector<float>(variablesTracked, 1), beliefVector, beliefSum = vector<float>(variablesTracked, 0), beliefProduct = vector<float>(variablesTracked, 1);
	int neighbourX, neighbourY, currentX = currentNode->getCoords()[0], currentY = currentNode->getCoords()[1];	
	float occupancyBelief = 0;
	vector<float> neighbourProduct = defaultVector;
	float marginalOccupied, marginalNonOccupied;
	if (DEBUG) {
		cout << "-- processing node : " << currentX << ", " << currentY << endl;
	}
	//loop througn neighbours of currentNode, calculate updated incoming message for each neighbour
	n = currentNode->getNeighbours();
	for (currentn = n.begin(); currentn != n.end(); currentn ++) {
		neighbourX = currentn->first->getCoords()[0];	
		neighbourY = currentn->first->getCoords()[1];
		
		neighbourProduct = defaultVector;	
		//get all incoming messages to neighbour except from currentnode
		nn = currentn->first->getNeighbours();
		for (currentnn = nn.begin(); currentnn != nn.end(); currentnn ++) {
			if (currentnn->first != currentNode) {
				//product of all incoming messages to neighbour
				this->variableProduct(&neighbourProduct, currentnn->second);
			}
		}
		//factor in current observation and hidden potential
//		occupancyBelief = this->processedEnvironment->getMapping(neighbourX, neighbourY);
		occupancyBelief = this->externalEnvironment->getMapping(neighbourX, neighbourY);
//		beliefVector.clear();
//		beliefVector.push_back(occupancyBelief);
//		beliefVector.push_back(1 - occupancyBelief);
//		this->variableProduct(&neighbourProduct, beliefVector);
		
//		this->variableProduct(&neighbourProduct, this->hiddenPotentials);

		//factor in observations and link potential
		occupancyBelief = this->externalEnvironment->getMapping(neighbourX, neighbourY);
		beliefVector.clear();
		beliefVector.push_back(occupancyBelief);
		beliefVector.push_back(1 - occupancyBelief);
		this->variableProduct(&neighbourProduct, beliefVector);

		this->variableProduct(&neighbourProduct, this->linkPotentials);
		
		//marginalise	
		marginalOccupied = neighbourProduct[0];	
		marginalNonOccupied = neighbourProduct[1];	
		neighbourProduct[0] = this->hiddenPotentials[0] * marginalOccupied + this->hiddenPotentials[1] * marginalNonOccupied;
		neighbourProduct[1] = this->hiddenPotentials[2] * marginalOccupied + this->hiddenPotentials[3] * marginalNonOccupied;
		//update message from neighbour to current node
		currentn->second = neighbourProduct;
		if (DEBUG) {		
			cout << neighbourX << ", " << neighbourY << " -> " 
			<< currentX << ", " << currentY 
			<< " : (normalised) " << neighbourProduct[0] << " " <<  neighbourProduct[1] << endl;
		}		
		//add processed incoming message to belief update vector
		this->variableSum(&beliefSum, neighbourProduct);
		this->variableProduct(&beliefProduct, neighbourProduct);
	}
		
	//factor in current node's beliefs
	occupancyBelief = this->processedEnvironment->getMapping(currentX, currentY);
	occupancyBelief = this->externalEnvironment->getMapping(currentX, currentY);
	if (DEBUG) {
		cout << "current belief: " << occupancyBelief << endl;
	}
	beliefVector.clear();
	beliefVector.push_back(occupancyBelief);
	beliefVector.push_back(1 - occupancyBelief);
	this->variableSum(&beliefSum, beliefVector);
//	this->variableProduct(&beliefProduct, beliefVector);

	//factor in current node's observation and link potential
	occupancyBelief = this->externalEnvironment->getMapping(currentX, currentY);
	if (DEBUG) {
		cout << "current obs: " << occupancyBelief << endl;
	}
	beliefVector.clear();
	beliefVector.push_back(occupancyBelief);
	beliefVector.push_back(1 - occupancyBelief);
	this->variableProduct(&beliefVector, this->linkPotentials);
	this->variableSum(&beliefSum, beliefVector);	
	this->variableProduct(&beliefProduct, beliefVector);

	this->variableNormalise(&beliefSum);
	this->variableNormalise(&beliefProduct);
	if (DEBUG) {
		//cout << "update " << currentX << ", " << currentY << " : " << beliefSum[0] << "\n" << endl;
		cout << "update " << currentX << ", " << currentY << " : " << beliefProduct[0] << "\n" << endl;
	}
	return beliefProduct[0];
//	return beliefSum[0];
};

void occupancy_grid::variableProduct(vector<float> * v1, vector<float> v2) {
	for (int i = 0; i < (int) v1->size(); i ++) {
		v1->at(i) *= v2[i];
	}
};

void occupancy_grid::variableSum(vector<float> * v1, vector<float> v2) {
	for (int i = 0; i < (int) v1->size(); i ++) {
		v1->at(i) += v2[i];
	}
};

void occupancy_grid::variableNormalise(vector<float> * beliefs) {
	float sum = 0;
	for (int i = 0; i < (int) beliefs->size(); i ++) {
		sum += beliefs->at(i);
	}
	for (int i = 0; i < (int) beliefs->size(); i ++) {
		beliefs->at(i) *= (1 / sum);
	}
};
/*
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
*/
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

void occupancy_grid::learnParameters(Environment * groundTruth) {
	bool atMinimum = false;
	float currentError = 1, newError = 0;
	vector<float> linkPotentialJump; //vectors for the value and direction to shift each parameter
	vector<float> hiddenPotentialJump; //vectors for the value and direction to shift each parameter
	float jumpValue = 0.1; //how much each iteration will adjust the parameters by
	unsigned int i;
	
	this->loopyBeliefPropagation();
	currentError = this->processedEnvironment->calculateError(groundTruth); //initial error with default parameters

	//determine which direction to jump in for each parameter
	
	//calculating link potentials
	for (i = 0; i < this->linkPotentials.size(); i++) {
		this->linkPotentials[i] += jumpValue;
		this->loopyBeliefPropagation();
		newError = this->processedEnvironment->calculateError(groundTruth);
		if (newError < currentError) {
			linkPotentialJump.push_back(jumpValue);
		}
		else {
			linkPotentialJump.push_back(-jumpValue);
		}
		currentError = newError;
	}
	
	//calculating hidden potentials
	for (i = 0; i < this->hiddenPotentials.size(); i++) {
		this->hiddenPotentials[i] += jumpValue;
		this->loopyBeliefPropagation();
		newError = this->processedEnvironment->calculateError(groundTruth);
		if (newError < currentError) {
			hiddenPotentialJump.push_back(jumpValue);
		}
		else {
			hiddenPotentialJump.push_back(-jumpValue);
		}
		currentError = newError;
	}

	//gradient descent on a concave likelihood function
	while (!atMinimum) {
		atMinimum = true;
		
		//link potential tuning
		for (i = 0; i < this->linkPotentials.size(); i++) {
			this->linkPotentials[i] += linkPotentialJump[i];
			this->loopyBeliefPropagation();
			newError = this->processedEnvironment->calculateError(groundTruth); 
			if (newError < currentError) {
				currentError = newError;
				atMinimum = false;
			}
			else {
				this->linkPotentials[i] -= linkPotentialJump[i];
			}
		}
		
		//hidden potential tuning
		for (i = 0; i < this->hiddenPotentials.size(); i++) {
			this->hiddenPotentials[i] += hiddenPotentialJump[i];
			this->loopyBeliefPropagation();
			newError = this->processedEnvironment->calculateError(groundTruth); 
			if (newError < currentError) {
				currentError = newError;
				atMinimum = false;
			}
			else {
				this->hiddenPotentials[i] -= hiddenPotentialJump[i];
			}
		}

	}

	cout << "occupied link potential = " << this->linkPotentials[0] << " empty link potential = " << this->linkPotentials[1] << " o-o = " << this->hiddenPotentials[0] << " o-e = " << this->hiddenPotentials[1] << " e-o =" << this->hiddenPotentials[2] << " e-e = " << this->hiddenPotentials[3] <<  " mse = " << currentError << endl;
};


void occupancy_grid::validation(Environment * groundTruth) {
	int x, y, truePositives = 0, trueNegatives = 0, falsePositives = 0, falseNegatives = 0;
	float trueOccupancy = 0.5, inferredOccupancy = 0.5;
	//loop through every cell and set it as unknown and then perform lbp
	for (x = 0; x < this->externalEnvironment->getGridSizeHorizontal(); x++) {
		for (y = 0; y < this->externalEnvironment->getGridSizeVertical(); y++) {
			trueOccupancy = this->externalEnvironment->getMapping(x, y);


			this->externalEnvironment->setMapping(x, y, 0.5);
			this->loopyBeliefPropagation();
			
			inferredOccupancy = this->processedEnvironment->getMapping(x, y);	
			//			cout << "testing: " << x << ", " << y << " - " << trueOccupancy << endl;
			if (x == 2 && y == 2) {
				cout << "inferred = " << inferredOccupancy << "true occupancy = " << trueOccupancy << endl;	
			}
			this->externalEnvironment->setMapping(x, y, trueOccupancy);

			if (inferredOccupancy > 0.5 && trueOccupancy > 0.5) {
				//true positive for occupied classification
				truePositives++;
			}
			else if (inferredOccupancy < 0.5 && trueOccupancy < 0.5) {
				//true negative for occupied classification
				trueNegatives++;
			}
			else if (inferredOccupancy < 0.5 && trueOccupancy > 0.5) {
				//false negative for occupied classification
				falseNegatives++;
			}
			else if (inferredOccupancy > 0.5 && trueOccupancy < 0.5) {
				//false positive for occupied classification
				falsePositives++;
			}
		}
	}
	
	cout << "truePositives = " << truePositives << " - trueNegatives = " << trueNegatives
	<< " - falsePositives = " << falsePositives << " - falseNegatives = " << falseNegatives << endl;

};
#endif
