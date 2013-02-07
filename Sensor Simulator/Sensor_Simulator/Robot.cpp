#include "Robot.h"
#include <math.h>
#include <sstream>
#include <algorithm>
using namespace std;

float getAngle(int x1, int y1, int x2, int y2);
float getDistance(int x1, int y1, int x2, int y2);
float convertNegativeAngles(float angle);
float convertAngleQuadrant(); //converts angle to be in first quadrant

float getAngle(int x1, int y1, int x2, int y2) { //returns the angle of point 2 relative to point 1
	/*
	float deltaX, deltaY, angle;
	deltaX = x2 - x1;
	deltaY = y2 - y1;

	//if (deltaX)
	//angle = abs((atan(deltaY / deltaX) * 180) / (atan(1.0) * 4));	
	//angle = convertNegativeAngles((atan(deltaY / deltaX)));	
	angle = (atan(deltaY / deltaX));
	//angle = abs(atan(deltaY / deltaX));	
	if (deltaX == 0) {
		if (deltaY > 0) {
			return atan(1.0) * 4;
		}
		else {
			return 0;
		}		
	}

 	if (deltaX > 0 && deltaY >= 0) {
		angle += atan(1.0) * 2;
	}
	if (deltaX <= 0 && deltaY > 0) {
		//angle += atan(1.0) * 4;
	}
	if (deltaX >= 0 && deltaY <= 0) {

	}
	if (deltaX <= 0 && deltaY <= 0) {
		//angle += 6 * atan(1.0);
	}
	return angle;
	*/
	float deltaX, deltaY, angle;
	deltaX = x2 - x1;
	deltaY = y2 - y1;
	float pi2 = atan(1.0) * 2;
	angle = 0;

 	if (deltaX > 0 && deltaY >= 0) {
		angle = pi2 + atan(deltaY / deltaX);
	}
	if (deltaX <= 0 && deltaY > 0) {
		angle = 2 * pi2 + atan(abs(deltaX) / deltaY);
	}
	if (deltaX >= 0 && deltaY < 0) {
		if (deltaX == 0) {
			angle = 0;
		}
		else {
			angle = atan(deltaX / abs(deltaY));
		}
	}
	if (deltaX < 0 && deltaY <= 0) {
		angle += 3 * pi2 + atan(abs(deltaY) / abs(deltaX));
	}
	return angle;
};
float getDistance(int x1, int y1, int x2, int y2) {
	float deltaX, deltaY;
	deltaX = x2- x1;
	deltaY = y2 - y1;

	return sqrt(pow(deltaX, 2) + pow(deltaY, 2));
};
float convertNegativeAngles(float angle) {
	float pi = atan(1.0) * 4;
	if (angle < 0) {
		angle += pi * 2;
		
	}
	return angle;
};


Environment Robot::triggerSensors(Environment *realEnv) { //returns this particular instance of what sensors see
	float pi = atan(1.0) * 4;
	float angleUpperbound, angleLowerbound, currentAngle, convertedAngle, distance;
	float angleIncrementRadians = this->angleIncrement / (180 / pi);
	float x, y, roundedX, roundedY;
	int arraySize = this->viewAngle / this->angleIncrement;
	vector<pair<float, float>> distances; //map of angle to distance
	vector<float> sortedDistances;
	vector<pair<float, float>>::iterator distanceIt;
	float* ranges;
	Environment tempEnv;

	angleUpperbound = convertNegativeAngles(fmod(this->getOrientation() + this->getViewAngle() / 2, (pi * 2)));
	if (angleUpperbound == 0) {
		angleUpperbound = pi * 2;
	}

	angleLowerbound = convertNegativeAngles(fmod(this->getOrientation() - this->getViewAngle() / 2, (pi * 2)));
	//cout << "orientation = " << this->getOrientation() * (180 / pi) << endl;
	//cout << "upperbound = " << angleUpperbound * (180 / pi) << endl;
	//cout << "lowerbound = " << angleLowerbound * (180 / pi) << endl;
	//cout << "increment = " << angleIncrementRadians * (180 / pi) << endl;

	for (currentAngle = 0;  currentAngle <  2 * pi; currentAngle += angleIncrementRadians) {//draw circle around the robot to get edges of sensor		
		//find the edge points of circle around robot using parametric equations
		x = this->getMaxViewDistance() * cos(currentAngle);
		y = -this->getMaxViewDistance() * sin(currentAngle);
		roundedX = floor(x + 0.5);
		roundedY = floor(y + 0.5);

		convertedAngle = convertNegativeAngles((pi - (currentAngle + pi / 2))); //converts the angle to be viewed from 1st quadrant increasing clockwise
		distance = rayCast(*realEnv, roundedX, roundedY);
		//check if angle is in range
		if (	(	
					angleUpperbound < angleLowerbound
					&& (convertedAngle < angleUpperbound || convertedAngle > angleLowerbound)
				)
				||
				(	
					angleUpperbound > angleLowerbound
					&& (convertedAngle < angleUpperbound && convertedAngle > angleLowerbound)
				)
			) {

			//cout << "point = " << roundedX << ", " << roundedY 
			//<< "\tangle = " << convertedAngle * (180 / pi) << "\tactual angle = " << (pi - (currentAngle + pi / 2)) * (180 / pi) 
			//<< "\tdistance = " << distance << endl;
			//if ( floor(convertedAngle * (180 / pi)) == 126.0) {
			//	cout << "here";
			//}
			distances.push_back(make_pair(convertedAngle, distance));
		}		
	}
	sort(distances.begin(), distances.end()); //to ROS message specifications

	//make new vector to store distances sorted by ascending angles
	for (distanceIt = distances.begin(); distanceIt != distances.end(); distanceIt++) {
		sortedDistances.push_back(distanceIt->second);
	}

	ranges = new float[sortedDistances.size()];
	memcpy(ranges, &sortedDistances.front(), sortedDistances.size() * sizeof( float)); //put vector into array to be communicated across ROS

	this->environment = this->discretiseReadings(angleLowerbound, angleUpperbound, angleIncrementRadians, 0, this->maxViewDistance, ranges); //discretise the sensor readings
	return this->environment;
};
float Robot::rayCast(Environment realEnv, int x, int y) { //implements Bresenham's line algorithm

	int currentx = this->getXPos(); 
	int currenty = this->getYPos();
	int dx = abs(x - currentx);
	int dy = abs(y - currenty);
	int sx, sy, error, temperror;
	int threshold = 0;
	if (x > currentx) {
		sx = 1;
	}
	else {
		sx = -1;
	}
	
	if (y > currenty) {
		sy = 1;
	}
	else {
		sy = -1;
	}
	error = dx - dy;

	while (currentx != x || currenty != y) {		
		
		temperror = error * 2;
		if (temperror > -dy) {
			error -= dy;
			currentx += sx;
			/*
			//if	(threshold < 1 &&
			if	(
				!(temperror < dx) &&
				realEnv.checkHashedMapping(currentx, currenty)
				) {
					error += dx; 
					currenty += sy;
					//threshold++;
			}*/

		}
		if (temperror < dx) {
			error += dx;
			currenty += sy;
			/*
			//if	(threshold < 1 &&
			if	(
				!(temperror > -dy) &&
				realEnv.checkHashedMapping(currentx, currenty)
				) {
					error -= dy;
					currentx += sx;
					//threshold++;
			}*/

		}
				
		if (realEnv.checkHashedMapping(currentx, currenty)) {
			//cout << "current x = " << currentx << "; current y = " << currenty << endl;
			return getDistance(currentx, currenty, this->getXPos(), this->getYPos());
		}

		//threshold = 0;
	}

	return -1;
};
vector<pair<int, int>> Robot::rayCast(int x, int y) {
	int currentx = this->getXPos(); 
	int currenty = this->getYPos();
	int dx = abs(x - currentx);
	int dy = abs(y - currenty);
	int sx, sy, error, temperror;
	int threshold = 0;
	vector<pair<int, int>> nodePath;

	if (x > currentx) {
		sx = 1;
	}
	else {
		sx = -1;
	}
	
	if (y > currenty) {
		sy = 1;
	}
	else {
		sy = -1;
	}
	error = dx - dy;

	while (currentx != x || currenty != y) {		
		
		temperror = error * 2;
		if (temperror > -dy) {
			error -= dy;
			currentx += sx;
		}
		if (temperror < dx) {
			error += dx;
			currenty += sy;
		}				
		//if (getDistance(this->getXPos(), this->getYPos(), currentx, currenty)) {
		//	//cout << "current x = " << currentx << "; current y = " << currenty << endl;
		//	return nodePath;
		//}
		nodePath.push_back(make_pair(currentx, currenty));
	}
	return nodePath;
};

/*float Robot::rayCast(Environment realEnv, int x, int y) { //implements Bresenham's line algorithm

	int currentx = x; 
	int currenty = y;
	int dx = abs(this->getXPos() - currentx);
	int dy = abs(this->getYPos() - currenty);
	int sx, sy, error, temperror;
	int threshold = 0;
	if (this->getXPos() > currentx) {
		sx = 1;
	}
	else {
		sx = -1;
	}
	
	if (this->getYPos() > currenty) {
		sy = 1;
	}
	else {
		sy = -1;
	}
	error = dx - dy;

	while (currentx != this->getXPos() || currenty != this->getYPos()) {		
		
		temperror = error * 2;
		if (temperror > -dy) {
			error -= dy;
			currentx += sx;
			if	(threshold < 1 &&
				!(temperror < dx) &&
				realEnv.checkHashedMapping(currentx, currenty)
				) {
					error += dx; 
					currenty += sy;
					threshold++;
			}

		}
		if (temperror < dx) {
			error += dx;
			currenty += sy;
			if	(threshold < 1 &&
				!(temperror > -dy) &&
				realEnv.checkHashedMapping(currentx, currenty)
				) {
					error -= dy;
					currentx += sx;
					threshold++;
			}

		}
				
		if (realEnv.checkHashedMapping(currentx, currenty)) {
			//cout << "current x = " << currentx << "; current y = " << currenty << endl;
			return getDistance(currentx, currenty, this->getXPos(), this->getYPos());
		}

		threshold = 0;
	}

	return -1;
};*/

float Robot::factorNoise(float occupancyProbability) {
	if (occupancyProbability == 0) {
		return 0.1;
	}	
	else {
		return 0.9;
	}

};
void Robot::turn(float angle) {
	float radians = angle * (atan(1.0) * 4) / 180;
	float temp = (float) atan(1.0) * 8;
	this->Orientation += radians;
	this->Orientation = fmod(this->Orientation, (float) atan(1.0) * 8);

}; //turn the robot: clockwise with a positive angle, anti-clockwise with a negative angle
bool Robot::move(Environment *realEnv, int x, int y) { //move the robot: foward with a positive distance, backwards with a negative distance. Returns false if robot cannot move there.
	int currentx, currenty;

	currentx = this->coords.first;
	currenty = this->coords.second;

	if (realEnv->checkHashedMapping(currentx + x, currenty + y) || //checks if robot can move to spot
		currentx + x >= realEnv->getGridSizeHorizontal() ||
		currenty + y >= realEnv->getGridSizeVertical() ||
		currentx + x < 0 ||
		currenty + x < 0
		) {
		return false;
	}
	else {
		this->coords = make_pair(currentx + x, currenty + y);
		realEnv->updateRobotLocation(currentx + x, currenty + y);
		this->getRobotEnvironment()->updateRobotLocation(currentx + x, currenty + y);
	}
	return true;
};
Environment Robot::discretiseReadings(float angleMin, float angleMax, float angleIncrement, float rangeMin, float rangeMax, float ranges[]) {
	
	bool debug = true;
	int range;
	float distance, angle, deltaX, deltaY;
	float pi = atan(1.0) * 4;
	int numRange = (angleMax - angleMin) / angleIncrement;
	vector<pair<int, int>> pathNodes;
	vector<pair<int, int>>::iterator nodeRange;
	bool emptyRange = false; //if the current angle being processed has a empty path
	map<pair<int, int>, pair<int, int>> occupancyCounter; //contains number of hits for occupied/nonoccuped for each node. nodeCoords : <occupied counter, non occupied counter>
	map<pair<int, int>, pair<int, int>>::iterator occupancyCounterIterator; //contains number of hits for occupied/nonoccuped for each node. nodeCoords : <occupied counter, non occupied counter>

	for (range = 0; range < numRange; range ++) {
		distance = floor(ranges[range]);
		angle = angleMin + angleIncrement * range;
		emptyRange = false;
		if (debug) {cout << "angle = " << angle * (180 / pi);}

		if (distance == -1 || distance > rangeMax) { //-1 represents unonccupied ray trace, distance > rangeMax is to account for rounding errors when drawing the circle perimeter during sensor scan
			distance = rangeMax;
			emptyRange = true;
			if (debug) {cout << "\tempty range ";}
		};

		//conditions for angles
		if (angle == 0) {
			deltaY = -distance * cos(angle);
			deltaX = 0;
		}
		if (angle > 0 && angle < pi / 2) {
			deltaY = -distance * cos(angle);
			deltaX = distance * sin(angle);
		}
		if (angle >= pi / 2 && angle < pi) {
			//angle = fmod(angle, pi);
			angle -= pi / 2;
			deltaY = distance * sin(angle);
			deltaX = distance * cos(angle);
		}
		if (angle >= pi * 2 && angle < pi * (3 / 2)) {
			//angle = fmod(angle, pi);
			angle -= (pi / 2) * 2;
			deltaX = -distance * sin(angle);
			deltaY = distance * cos(angle);
		}
		if (angle >= pi * (3 / 2) && angle < pi * 2) {
			//angle = fmod(angle, pi);
			angle -= pi * (3 / 2);
			deltaY = -distance * sin(angle);
			deltaX = -distance * cos(angle);
		}
		/*deltaX = floor(deltaX * 100) / 100;
		deltaY = floor(deltaY * 100) / 100;*/
		deltaX = floor(deltaX + 0.5);
		deltaY = floor(deltaY + 0.5);
		

		if (debug) {cout << "\tdistance = " << floor(distance * 100) / 100 << "\tdeltaX = " << deltaX << "\tdeltaY = " << deltaY << endl;}
		pathNodes = rayCast(deltaX, deltaY);

		for (nodeRange = pathNodes.begin(); nodeRange != pathNodes.end(); nodeRange ++) {
			occupancyCounterIterator = occupancyCounter.find(*nodeRange);
			if (occupancyCounterIterator != occupancyCounter.end()) {
				if (!emptyRange && nodeRange + 1 == pathNodes.end()) { //last node in range should be recorded as occupied in a non empty range
					occupancyCounter[*nodeRange].first += 1; //increment occupied count
				}
				else {
					occupancyCounter[*nodeRange].second += 1; //increment unoccupied count
				}
			}
			else {
				if (!emptyRange && nodeRange + 1 == pathNodes.end()) { //last node in range should be recorded as occupied in a non empty range
					occupancyCounter[*nodeRange].first = 1;
				}
				else {
					if (nodeRange->first == 2 && nodeRange->second == 5) {
						cout << "here";
					}
					occupancyCounter[*nodeRange].second = 1;
				}
			}				
		}
		
		//pathNodes = rayCast(2, 0);
	}
	for (occupancyCounterIterator = occupancyCounter.begin(); occupancyCounterIterator != occupancyCounter.end(); occupancyCounterIterator ++) { //put occupancy values into mapping
		this->environment.setMapping(this->getXPos() + occupancyCounterIterator->first.first, this->getXPos() + occupancyCounterIterator->first.second, 
									occupancyCounterIterator->second.first - occupancyCounterIterator->second.second);
	}
	//this->environment.setMapping(1, 0, 2);
	//this->environment.setMapping(1, 0, 3);
	//this->environment.addHashedMapping2(1, 0);
	return this->environment;
}
Environment Robot::discretiseReadings(Environment realEnv, map<pair<float, float>, float> angleDistanceMap) {
	Environment newEnv = new Environment(realEnv);
	map<pair<float, float>, float>::iterator it;
	float angle, distance, occupancyProbability;
	float deltaX, deltaY;
	float pi = atan(1.0) * 2;
	for (it = angleDistanceMap.begin(); it != angleDistanceMap.end(); it++) {
		angle = it->first.first;
		//cout << angle * 180 / (pi * 2);
		distance = it->first.second;
		occupancyProbability = it->second;

		//conditions for angles
		if (angle == 0) {
			deltaY = -distance * cos(angle);
			deltaX = 0;
		}
		if (angle > 0 && angle < pi) {
			deltaY = -distance * cos(angle);
			deltaX = distance * sin(angle);
		}
		if (angle >= pi && angle < pi * 2) {
			//angle = fmod(angle, pi);
			angle -= pi;
			deltaY = distance * sin(angle);
			deltaX = distance * cos(angle);
		}
		if (angle >= pi * 2 && angle < pi * 3) {
			//angle = fmod(angle, pi);
			angle -= pi * 2;
			deltaX = -distance * sin(angle);
			deltaY = distance * cos(angle);
		}
		if (angle >= pi * 3 && angle < pi * 4) {
			//angle = fmod(angle, pi);
			angle -= pi * 3;
			deltaY = -distance * sin(angle);
			deltaX = -distance * cos(angle);
		}
		deltaX = floor(deltaX + 0.1);
		deltaY = floor(deltaY + 0.1);
		newEnv.addHashedMapping2(this->getXPos() + deltaX, this->getYPos() + deltaY);
		newEnv.setMapping(this->getXPos() + deltaX, this->getYPos() + deltaY, occupancyProbability);
		this->addSensorHistory(this->getXPos() + deltaX, this->getYPos() + deltaY, occupancyProbability);
		//cout << "x = " << this->getXPos() + deltaX << "; y = " << this->getYPos() + deltaY << "; probability = " << occupancyProbability << endl;
	}


	return newEnv;
};

