#include "occupancy_grid.h"
//#include <unistd.h>
using namespace std;

void crf_thread();
int main() {
	//pthread_t crf_thread;
	vector<int> gridSize;
	gridSize.push_back(10); //horizontal
	gridSize.push_back(5); //vertical

	occupancy_grid *crf = new occupancy_grid(2, gridSize);

	cin.ignore();
	return 0;
};

void crf_thread() {
	
};