#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <algorithm>
#include <time.h>
#include "Environment.h"
using namespace std;
float negativeInverseExponential(float x);

float negativeInverseExponential(float x) {
	return -exp(-x - 2.5) + 0.1;
};

Environment::Environment() {
	this->gridSizeHorizontal = 14;
	this->gridSizeVertical = 14;
	this->robotCoords = make_pair(0, 0);	
	this->generateTestMap();
	this->occupancyValueThreshold = 0.3;
};

Environment::Environment(Environment *env) {
	this->gridSizeHorizontal = env->gridSizeHorizontal;
	this->gridSizeVertical = env->gridSizeVertical;
	this->robotCoords = env->robotCoords;
	this->mapping = env->getMap();
	this->occupancyValueThreshold = 0.3;
};

Environment::Environment(bool unknownMap) {
	this->gridSizeHorizontal = 14;
	this->gridSizeVertical = 14;
	this->robotCoords = make_pair(0, 0);	
	this->occupancyValueThreshold = 0.3;
	if (unknownMap) {
		this->generateUnknownMap();
	}
	else {
		this->generateTestMap();
	}
};

void Environment::setMap(map<pair<int, int>, float> newMap) {
	this->mapping = newMap;	
};

float Environment::getOccupancyValueThreshold() {
	return this->occupancyValueThreshold;
};

map<pair<int, int>, float> Environment::getMap() {
	return this->mapping;
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

//	this->setMapping(2, 1, 1);
//	addHashedMapping(2, 1);
//	this->setMapping(2, 2, 1);
//	addHashedMapping(2, 2);

	this->setMapping(2, 3, 1);
	addHashedMapping(2, 3);
	this->setMapping(2, 4, 1);
	addHashedMapping(2, 4);
	//this->setMapping(2, 5, 1);
	//addHashedMapping(2, 5);
	//this->setMapping(2, 6, 1);
	//addHashedMapping(2, 6);
	
	//testing set for hardcoded wall
	/*
	this->setMapping(1, 8, 1);
	addHashedMapping(1, 8);
	this->setMapping(2, 8, 1);
	addHashedMapping(2, 8);
	this->setMapping(3, 8, 1);
	addHashedMapping(3, 8);	
	this->setMapping(4, 8, 1);
	addHashedMapping(4, 8);
	this->setMapping(5, 8, 1);
	addHashedMapping(5, 8);	
	this->setMapping(6, 9, 1);
	addHashedMapping(6, 9);	
	this->setMapping(7, 8, 1);
	addHashedMapping(7, 8);	
	this->setMapping(8, 9, 1);
	addHashedMapping(8, 9);
	*/
//	srand(time(NULL));
	srand(2);
	
	//populate randomly
/*	
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
*/
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
				cout << floor(this->getMapping(x, y) * 100) / 100 << "\t";
			}
		}
		cout << endl << endl;
	}
};

/*void Environment::mergeMapping(Environment newEnv, map<pair<int, int>, list<float> > sensorHistory) {
	unordered_set<string> hashedMapping = newEnv.getHashedMapping2();
	unordered_set<string>::iterator it;
	int x, y;

	for (it = hashedMapping.begin(); it != hashedMapping.end(); it ++) {
		stringstream((*it).substr(0, (*it).find(" "))) >> x;
		stringstream((*it).substr((*it).find(" "))) >> y;
		this->setMapping(x, y, calculateNewCertainty(sensorHistory[make_pair(x, y)], newEnv.getMapping(x, y)));
	}

};*/

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

void Environment::writeToFile(string fileName) {
	int x, y;
	ofstream myFile;
//	std::string fullPath = "__" + std::string(WRITEFILEPATH) + fileName;
	std::string fullPath = "__" + fileName;
	remove(fullPath.c_str());
	myFile.open(fullPath);

//	myFile << "X = [";
	for (y = 0; y < this->getGridSizeHorizontal(); y ++) {
		for (x = 0; x < this->getGridSizeVertical(); x ++) {
			myFile << this->getMapping(x, y) << ",";
		}
		myFile << ";";
	}
//	myFile << "];";
	myFile.close();

//	cout << "wrote map to file: " << fullPath << endl;
};

void Environment::readFromFile(string fileName) {
	string line;
	ifstream f(fileName);
	std::vector<std::string> rows;
	std::vector<std::string> pixels;
	unsigned int x, y;
	if (f.is_open()) {
		getline(f, line);	
		boost::split(rows, line, boost::is_any_of(";"));
		for (y = 0; y < rows.size(); y ++) {
			boost::split(pixels, rows[y], boost::is_any_of(","));
			for (x = 0; x < pixels.size(); x++) {
//				cout << pixels[x];
				this->setMapping(x, y, atof(pixels[x].c_str()));
//				cout <<  atof(pixels[x].c_str());
			}
		}
	}
	this->gridSizeHorizontal = rows.size() - 1;
	this->gridSizeVertical = rows.size() - 1;
};

float Environment::calculateError(Environment * env) {
	int x, y;
	float errorTotal = 0; 
	for (x = 0; x < this->gridSizeHorizontal; x++) {
		for (y = 0; y < this->gridSizeVertical; y++) {
			errorTotal += pow(env->getMapping(x, y) - this->getMapping(x, y), 2);
		}
	}
	
	return errorTotal / (this->gridSizeHorizontal * this->gridSizeVertical);
};
#endif
