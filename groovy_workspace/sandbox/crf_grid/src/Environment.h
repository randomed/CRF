#include <map>
#include <iostream>
#include <unordered_set>
#include <string>
#include <sstream>
#include <list>
using namespace std;

class Environment {
protected:	
	int gridSizeHorizontal;
	int gridSizeVertical;
	pair<int, int> robotCoords;
	map<pair<int, int>, float> mapping;	//represents the environment, mapping coordinates to the probability node is occupied
	unordered_set<string> hashedMapping; //for fast lookup during sensor operations
//	unordered_set<string> hashedMapping2; //for fast lookup during newly detect cells
	float calculateNewCertainty(list<float> cellHistory, float newOccupancy);
	float occupancyValueThreshold;
public:
	Environment();	
	Environment(Environment *env); 
	void generateTestMap();
	void generateUnknownMap();
	void printMap();

	void setMapping(int x, int y, float probability) {
		this->mapping[make_pair(x, y)] = probability;
	};

	unordered_set<string> getHashedMapping() {return this->hashedMapping;};
//	unordered_set<string> getHashedMapping2() {return this->hashedMapping2;};
	void updateRobotLocation(int x, int y) {
		this->robotCoords = make_pair(x, y);
	};

	int getGridSizeHorizontal() {
		return this->gridSizeHorizontal;
	};
	int getGridSizeVertical() {
		return this->gridSizeVertical;
	};
	float getMapping(int x, int y) {
		return this->mapping[make_pair(x, y)];
	};
	float getOccupancyValueThreshold();
	bool checkHashedMapping(int x, int y) { //checks if space is occupied, prevents algorithm from extending beyond borders
		if (x < 0 || y < 0 || x > this->gridSizeHorizontal - 1 || y > this->gridSizeVertical - 1) {
			return true;
		}
		stringstream ss;
		ss << x << " " << y;		
		return (!this->hashedMapping.count(ss.str()) == 0);
	};
	void addHashedMapping(int x, int y) {
		stringstream ss;
		ss << x << " " << y;
		this->hashedMapping.insert(ss.str());
	};

/*	void addHashedMapping2(int x, int y) {
		stringstream ss;
		ss << x << " " << y;
		this->hashedMapping2.insert(ss.str());
	};*/
//	void mergeMapping(Environment newEnv, map<pair<int, int>, list<float> > sensorHistory);
	int getRobotX() { return this->robotCoords.first;};
	int getRobotY() { return this->robotCoords.second;};
	void clearHashedMapping() {this->hashedMapping.clear();};
	void setMap(map<pair<int, int>, float> newMap);
	map<pair<int, int>, float> getMap();
//	void clearHashedMapping2() {this->hashedMapping2.clear();};
};

