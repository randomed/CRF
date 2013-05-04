#include <stdlib.h>
#include <stdio.h>
#include <unordered_map>
#include <math.h>
#include <iostream>
#include <iterator>
#include <queue>
#include "Environment.cpp"
#include "node.cpp"
#include<boost/functional/hash.hpp>
#include <map>

template <typename Container> //container for hashing unordered map with vector as a key
struct container_hash {
	    std::size_t operator()(Container const& c) const {
			         return boost::hash_range(c.begin(), c.end());
		 }
};

class occupancy_grid {
protected:
	int dimensions; //default dimension is 2;
	vector<int> gridSize; //grid size, indexed by dimensions
	unordered_map<vector<int>, node*, container_hash<vector<int>>> grid; //contains the grid, vectors of the coodinates mapping to the node
//	map<vector<int>, node*> grid; //contains the grid, vectors of the coodinates mapping to the node
	int iterationCount; //number of iterations for loopy belief propagation
	Environment * externalEnvironment; //environment passed in to be processed
	Environment * processedEnvironment; //copy of the original environment which has been processed
	vector<float> linkPotentials; //potentials between grid and observations
	vector<float> hiddenPotentials; //potentials between nodes on the grid
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
	void validation(Environment * groundTruth); //cross validation
	void learnParameters(Environment * groundTruth); //gradient descent of mean square error to learn pairwise potentials
	void incrementHiddenPotential(float jump);
	void setHiddenPotentials(vector<float> potentials);
	void setLinkPotentials(vector<float> potentials);
private:
	void __constructGrid(node * currentNode); //recusive function to iterate through all coordinates
	float calculateIncomingMessages(node * currentNode); //calculates the product of all incoming messages from all function node to variable node
//	vector<vector<int>> getNeighbours(vector<int> currentCoords); //get coordinates of the neighbours of given node
//	float calculateIncomingMessages(vector<int> currentNode); //calculates the product of all incoming messages from all function node to variable node
//	float ** __processedMessages1; //all the messages going towards the bottom right of grid
//	float ** __processedMessages2; //all the messages going towards the upper left of the grid

//	float getMessage(vector<int> neighbourNode, vector<int> currentNode);
//	void setMessage(vector<int> neighbourNode, vector<int> currentNode, float message);
	
	float normalise(float val1, float val2); //normalises to add to 1, and returns normalised value for val1	
	void variableProduct(vector<float> * v1, vector<float> v2); //in place element wise multiplication of 2 vectors, modifies v1	
	void variableSum(vector<float> * v1, vector<float> v2); //in place element wise addition of 2 vectors, modifies v1
	void variableNormalise(vector<float> * beliefs); //in place normalisation of the contents of beliefs to add to 1
};
