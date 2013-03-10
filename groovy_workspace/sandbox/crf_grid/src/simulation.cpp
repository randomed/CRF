#include "occupancy_grid.h"

int main() {
	int iteration;
	vector<int> gridSize;
	gridSize.push_back(10); //horizontal
	gridSize.push_back(10); //vertical
	Environment * env = new Environment();
	env->generateUnknownMap();
	//occupancy_grid *crf = new occupancy_grid(2, gridSize, 4);	
	occupancy_grid *crf = new occupancy_grid(env, 2);

	env->setMapping(1, 0, 0.2);
	env->setMapping(7, 3, 0.8);
	//env->setMapping(6, 3, 0.6);
	//crf->getNode(0, 0)->setFixedValue(0);
	//crf->getNode(7, 3)->setFixedValue(0.9);
	//crf->getNode(6, 3)->setFixedValue(0.1);
	//crf->getNode(7, 3)->setFixedValue(0.9);
	//crf->getNode(8, 2)->setFixedValue(0.9);
	cout <<  "--initial grid--" << endl;
	env->printMap();
	
	crf->setIterationCount(30);

	crf->loopyBeliefPropagation();
	/*
	cout << "setting cell (0, 5) value to 0.6" << endl;
	env->setMapping(0, 5, 0.6);
	crf->loopyBeliefPropagation();
	*/
	cin.ignore();
	return 0;
};