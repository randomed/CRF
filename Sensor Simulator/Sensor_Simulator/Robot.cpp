#include "Robot.h"
#include <math.h>
#include <sstream>
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
	/*
	Environment robotEnv = new Environment(realEnv);
	unordered_set<string>::iterator it;
	unordered_set<string> tempMap = realEnv->getHashedMapping();
	int tempX, tempY, x, y;
	float angleUpperbound, angleLowerbound, tempAngle;
	Environment tempEnv;
	stringstream ss;
	float pi = atan(1.0) * 4;
	bool pass = false;
	float temp;
	map< int, int> pairs = *new map<int, int>();
	*/
	/*
	//cout << "result: " << rayTrace(realEnv, 4, 3) << endl;

	
	for (it = tempMap.begin(); it != tempMap.end(); it++) {
		stringstream((*it).substr(0, (*it).find(" "))) >> tempX;
		stringstream((*it).substr((*it).find(" "))) >> tempY;
		if (rayCast(realEnv, tempX, tempY)) { //add node if there is line of sight with the robot
			robotEnv.setMapping(tempX, tempY, factorNoise());
		}
	}
	*/
	/*
	angleUpperbound = convertNegativeAngles(fmod(this->getOrientation() + this->getViewAngle() / 2, (pi * 2)));
	if (angleUpperbound == 0) {
		angleUpperbound = pi * 2;
	}
	angleLowerbound =  convertNegativeAngles(fmod(this->getOrientation() - this->getViewAngle() / 2, pi * 2));	

	for (x = this->getXPos() - this->getMaxViewDistance(); x < this->getXPos() + this->getMaxViewDistance() ; x++) {		
		for (y = this->getYPos() - this->getMaxViewDistance(); y <this->getYPos() + this->getMaxViewDistance() ; y++) {
	//for (x = this->getXPos(); x < this->getXPos() + this->getMaxViewDistance() ; x++) {		
		//for (y = this->getYPos(); y <this->getYPos() + this->getMaxViewDistance() ; y++) {
			tempAngle = getAngle(this->getXPos(), this->getYPos(), x, y);
			ss << x << " " << y;
			if (x == 1 && y == 0) {
				//cout << tempAngle * 180 / pi;
				//cout << angleUpperbound * 180 / pi;
				tempAngle = getAngle(this->getXPos(), this->getYPos(), x, y);
				
			}
			if (x == 4 && y == 7) {
				//cout << tempAngle * 180 / pi;
				//cout << angleUpperbound * 180 / pi;
				tempAngle = getAngle(this->getXPos(), this->getYPos(), x, y);				
			}
			
			if (angleUpperbound > angleLowerbound) { //checks angle cases	
				if (floor(tempAngle * 1000) <= floor(angleUpperbound * 1000)
					&& floor(tempAngle * 1000) >= floor(angleLowerbound * 1000)) {
						pass = true;
				}
				
			}
			else {
				if (floor(tempAngle * 1000) >= floor(angleLowerbound * 1000)
					|| floor(tempAngle * 1000) <= floor(angleUpperbound * 1000)) {
						pass = true;
				}
			}
			if (tempAngle == 0 && angleUpperbound == 2 * pi) { //checks for specific case where point is directly north
				pass = true;
			}

			if (pass
				&& x >= 0 && y >= 0 
				&& x < realEnv->getGridSizeHorizontal()
				&& y < realEnv->getGridSizeVertical()
				&& rayCast(*realEnv, x, y)) 
			{				
				this->addAngleDistanceMapping(tempAngle, getDistance(this->getXPos(), this->getYPos(), x, y), factorNoise(realEnv->getMapping(x, y))); //factor noise
				//cout << "x = " << x << "; y = " << y << "; angle = " << tempAngle << endl;
			}
			ss.str("");
			pass = false;
		}
	}

	tempEnv = this->discretiseReadings(*realEnv, this->getAngleDistanceMap()); //discretise the sensor readings
	this->getRobotEnvironment()->mergeMapping(tempEnv, this->getSensorHistory()); //add new sensor readings to current map
	this->clearAngleDistanceMap();
	realEnv->clearHashedMapping2();
	//cout << "currentx = " << this->getXPos() << "currenty = " << this->getYPos() << endl;
	//return tempEnv;
	*/
	float pi = atan(1.0) * 4;
	float angleUpperbound, angleLowerbound, currentAngle, convertedAngle, distance;
	float angleIncrementRadians = this->angleIncrement / (180 / pi);
	float x, y, roundedX, roundedY;
	Environment tempEnv;

	//angleUpperbound = fmod(this->getOrientation() + this->getViewAngle() / 2, (pi * 2));
	//if (angleUpperbound == 0) {
	//	angleUpperbound = pi * 2;
	//}
	//angleLowerbound =  fmod(this->getOrientation() - this->getViewAngle() / 2, pi * 2);	
	


	/*for (currentAngle = angleUpperbound + (pi / 2);  currentAngle > angleLowerbound + (pi / 2); currentAngle -= angleIncrementRadians) {
		x = 10 * cos(currentAngle);
		y = 10 * sin(currentAngle);
		cout << "point = " << floor(x * 100) / 100 << ", " << floor(y * 100) / 100 << endl;
	}*/
	//angleUpperbound = convertNegativeAngles(pi - fmod(this->getOrientation() + this->getViewAngle() / 2, (pi * 2)) + (pi / 2));
	angleUpperbound = convertNegativeAngles(fmod(this->getOrientation() + this->getViewAngle() / 2, (pi * 2)));
	if (angleUpperbound == 0) {
		angleUpperbound = pi * 2;
	}
	//angleLowerbound = convertNegativeAngles((pi - fmod(this->getOrientation() - this->getViewAngle() / 2, (pi * 2)) + (pi / 2)));
	angleLowerbound = convertNegativeAngles(fmod(this->getOrientation() - this->getViewAngle() / 2, (pi * 2)));
	cout << "orientation = " << this->getOrientation() * (180 / pi) << endl;
	cout << "upperbound = " << angleUpperbound * (180 / pi) << endl;
	cout << "lowerbound = " << angleLowerbound * (180 / pi) << endl;
	cout << "increment = " << angleIncrementRadians * (180 / pi) << endl;

	for (currentAngle = 0;  currentAngle <  2 * pi; currentAngle += angleIncrementRadians * 3) { //converted to angles so there are less rounding errors (in angleIncrementRadians) after many iterations
	//for (currentAngle = angleLowerbound;  currentAngle <  angleUpperbound; currentAngle += angleIncrementRadians * 3) { //converted to angles so there are less rounding errors (in angleIncrementRadians) after many iterations
		
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
			this->addAngleDistanceMapping(convertedAngle, distance, realEnv->getMapping(x, y)); //factor noise
			cout << "point = " << roundedX << ", " << roundedY 
			<< "\tangle = " << convertedAngle * (180 / pi) << "\tactual angle = " << (pi - (currentAngle + pi / 2)) * (180 / pi) 
			<< "\tdistance = " << distance << endl;
		}			

		
	}

	//##for testing, delete later
	tempEnv = this->discretiseReadings(*realEnv, this->getAngleDistanceMap()); //discretise the sensor readings
	//this->getRobotEnvironment()->mergeMapping(tempEnv, this->getSensorHistory()); //add new sensor readings to current map
	this->environment = tempEnv;
	this->clearAngleDistanceMap();
	realEnv->clearHashedMapping2();
	return tempEnv;

	return this->getRobotEnvironment();
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

