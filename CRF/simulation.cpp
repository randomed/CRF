#include "occupancy_grid.h"
//#include <unistd.h>
using namespace std;

void crf_thread();
int main() {
	//pthread_t crf_thread;
	int iteration;
	vector<int> gridSize;
	gridSize.push_back(10); //horizontal
	gridSize.push_back(10); //vertical

	node * testNode;

	occupancy_grid *crf = new occupancy_grid(2, gridSize, 4);	
	crf->getNode(0, 0)->setValue(0.9);
	cout <<  "--initial grid--" << endl;
	crf->printGrid();

	
	crf->setIterationCount(1);
	for (iteration = 0; iteration < 4; iteration ++) { //here to debug each iteration seperately
		cout <<  "--new iteration (" << iteration + 1 << ")--" << endl;
		crf->loopyBeliefPropagation();
		crf->printGrid();
		cout << endl;
	}
	cin.ignore();
	return 0;
};

void crf_thread() {
	
};