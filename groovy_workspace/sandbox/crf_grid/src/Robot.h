#include <iostream>
#include <stdlib.h>
#include <map>
#include "Environment.cpp"
#include "nav_msgs/Odometry.h"
#include "sensor_msgs/LaserScan.h"
#include "ros/ros.h"
#include "rviz_publish.cpp"
#define ROBOTGRIDVIEWTOPIC "robot_grid_view"
using namespace std;

class Robot {
protected:
	float Orientation; //orientation of the robot and its x and y coordinates on the mapping
	pair<int, int> coords;
	Environment environment; //The robot's view of the environment
	float rayCast(Environment realEnv, int x, int y); //performs ray tracing from the point of the robot to the x, y paramters. Returns the distance at which a occupied point is discovered, -1 if no occupied detected
	vector<pair<int, int>> rayCast(int x, int y); //gets all nodes in the path from the robot to (x, y)
	//map<pair<float, float>, float> angleDistanceMap; //maps sensor readings of angle and distances to what the sensor detected
	map<pair<int, int>, list<float>> sensorHistory; 
	float viewAngle;
	int maxViewDistance;
	float angleIncrement; //how much to increase the angle by at each reading, in degrees
	nav_msgs::Odometry odometry;
	sensor_msgs::LaserScan laserScan;
public:
	Robot() {
//		float pi = (atan(1.0) * 4);

		this->Orientation = 180;

		this->Orientation *= (atan(1.0) * 4) / 180;
		this->coords = make_pair(0, 0);

		viewAngle = 180;

		this->viewAngle *= (atan(1.0) * 4) / 180;

		maxViewDistance = 5;

		this->environment = new Environment(*new Environment());

		this->angleIncrement = 3;
	};
	Robot(int x, int y) {
		this->Orientation = 90;
		this->Orientation *= (atan(1.0) * 4) / 180;
		this->coords = make_pair(x, y);
		this->viewAngle = 180;
		this->viewAngle *= (atan(1.0) * 4) / 180;
		maxViewDistance = 5;
		this->angleIncrement = 3;
	};

	//map<pair<float, float>, float> getAngleDistanceMap() {return this->angleDistanceMap;};
	//void addAngleDistanceMapping(float angle, float distance, float probability) {this->angleDistanceMap[make_pair(angle, distance)] = probability;};
	float getViewAngle() {return this->viewAngle;};
	int getMaxViewDistance() {return this->maxViewDistance;};
	float getOrientation() {return this->Orientation;};
	void turn(float angle); //turn the robot: clockwise with a positive angle, anti-clockwise with a negative angle
	bool move(Environment *realEnv, int x, int y); //move the robot: foward with a positive distance, backwards with a negative distance
	Environment triggerSensors(Environment *realEnv); //returns a mapping of the part of the environment the robot can see, takes in the real environment
	void setRobotEnvironment(Environment robotEnv) {this->environment = robotEnv;};
	Environment *getRobotEnvironment() {return &this->environment;};
	Environment discretiseReadings(Environment realEnv, map<pair<float, float>, float> angleDistanceMap);//##outdated##
	Environment discretiseReadings(float angleMin, float angleMax, float angleIncrement, float rangeMin, float rangeMax, const float ranges[]);
	//void clearAngleDistanceMap() {this->angleDistanceMap.clear();};
	int getXPos() {return this->coords.first;};
	int getYPos() {return this->coords.second;};
	sensor_msgs::LaserScan getLaserScan() {return this->laserScan;};
	float factorNoise(float occupancyProbability);
	map<pair<int, int>, list<float>> getSensorHistory() {return this->sensorHistory;};
	void addSensorHistory(int x, int y, float occupancyProbability) {this->sensorHistory[make_pair(x, y)].push_back(occupancyProbability);};
	float normaliseOccupancy(float occupancyProbability);
	void processLaserScan(const sensor_msgs::LaserScan::ConstPtr& msg); //extract LaserScan message from topic and discretise the readings
};
