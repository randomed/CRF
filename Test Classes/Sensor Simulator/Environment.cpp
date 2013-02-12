#include "Environment.h"
#include <sstream>
#include <algorithm>
#include <time.h>

using namespace std;
float negativeInverseExponential(float x);

float negativeInverseExponential(float x) {
	return -exp(-x - 2.5) + 0.1;
};

void Environment::generateTestMap() {
	int x, y;
	stringstream ss;

	for (x = 0; x < this->gridSizeHorizontal; x ++) {
		for (y = 0; y < this->gridSizeVertical; y ++) {
			this->setMapping(x, y, 0);
		}
	}	

	//add hardcoded features here
	
	//for (x = 3; x < 7; x ++) {
	//	for (y = 2; y < 4; y ++) {
	//		this->setMapping(x, y, 1);
	//		addHashedMapping(x, y);
	//	}
	//}	
	//for (x = 1; x < 3; x ++) {
	//	for (y = 5; y < 8; y ++) {
	//		this->setMapping(x, y, 1);
	//		addHashedMapping(x, y);
	//	}
	//}	

	//this->setMapping(2, 1, 1);
	//addHashedMapping(2, 1);
	//this->setMapping(2, 2, 1);
	//addHashedMapping(2, 2);

	//this->setMapping(2, 3, 1);
	//addHashedMapping(2, 3);
	//this->setMapping(2, 4, 1);
	//addHashedMapping(2, 4);
	//this->setMapping(2, 5, 1);
	//addHashedMapping(2, 5);
	//this->setMapping(2, 6, 1);
	//addHashedMapping(2, 6);
	
	srand(time(NULL));

	//populate randomly
	int randomx, randomy;
	y = 0;
	x = 0;
	for (int i = 0; i < 2; i ++) {
		x = rand() % (this->gridSizeHorizontal - 1);
		//randomx = 1 + x + (rand() % (this->gridSizeHorizontal - x));
		randomx =  1 + x + rand() % 10;
		for (;x < randomx; x ++) {
			y = rand() % (this->gridSizeVertical - 1);
			//randomy = 1 + y + (rand() % (this->gridSizeVertical - y));
			randomy =  1 + y + rand() % 10;
			for (;y < randomy; y ++) {
				this->setMapping(x, y, 1);
				addHashedMapping(x, y);
			}
		}
	}	
};

void Environment::generateUnknownMap() {
	int x, y;

	for (x = 0; x < this->gridSizeHorizontal; x ++) {
		for (y = 0; y < this->gridSizeVertical; y ++) {
			this->setMapping(x, y, 0.5);
		}
	}	
};
void Environment::printMap() {
	int x, y;
	//label x axes
	cout << "\t";
	for (x = 1; x < this->gridSizeHorizontal + 1; x ++) {
		cout << "-" << x - 1 << "-\t";
	}
	cout << endl;

	for (y = 0; y < this->gridSizeVertical; y ++) {

		cout << "-" << y  << "-\t";

		for (x = 0; x < this->gridSizeHorizontal; x ++) {
			if (x == this->robotCoords.first && y == this->robotCoords.second) {
				cout << "R" << "\t";
			}
			else {
				cout << this->getMapping(x, y) << "\t";
			}
		}
		cout << endl << endl;
	}
};

void Environment::mergeMapping(Environment newEnv, map<pair<int, int>, list<float>> sensorHistory) {
	unordered_set<string> hashedMapping = newEnv.getHashedMapping2();
	unordered_set<string>::iterator it;
	int x, y;

	for (it = hashedMapping.begin(); it != hashedMapping.end(); it ++) {
		stringstream((*it).substr(0, (*it).find(" "))) >> x;
		stringstream((*it).substr((*it).find(" "))) >> y;
		this->setMapping(x, y, calculateNewCertainty(sensorHistory[make_pair(x, y)], newEnv.getMapping(x, y)));
	}

};

float Environment::calculateNewCertainty(list<float> cellHistory, float newOccupancy) {
	int viewCount;

	viewCount = count(cellHistory.begin(), cellHistory.end(), newOccupancy);	
	if (newOccupancy < 0.5) {
		return floor((newOccupancy - negativeInverseExponential(viewCount)) * 100) / 100;
	}
	else {
		return floor((newOccupancy + negativeInverseExponential(viewCount)) * 100) / 100;
	}

};