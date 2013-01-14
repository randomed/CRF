#include "occupancy_grid.h"
//#include <unistd.h>
using namespace std;

void crf_thread();
int main() {
	//pthread_t crf_thread;
	vector<int> gridSize;
	gridSize.push_back(2); //horizontal
	gridSize.push_back(2); //vertical

	node * testNode;

	occupancy_grid *crf = new occupancy_grid(2, gridSize);

	//testNode = crf->getNode(3,2);
	crf->printGrid();
	cin.ignore();
	return 0;
};

void crf_thread() {
	
};