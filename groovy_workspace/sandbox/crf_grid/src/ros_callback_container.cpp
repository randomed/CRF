#include "Robot.cpp"
#include "occupancy_grid.cpp"
#include "ros/ros.h"
class callbackContainer {

private:
	Robot * robot;
	occupancy_grid * gridOperations;
public:

	callbackContainer() {};
	void setRobot(Robot * robot) {
		this->robot = robot;
	}
	
	void setOccupancyGrid(occupancy_grid* gridOperations) {
		this->gridOperations = gridOperations;
	};
	void processLaserScan(const sensor_msgs::LaserScan::ConstPtr& msg) {
		ROS_INFO("starting processing");
		this->robot->processLaserScan(msg);
		cout << "unprocessed:"<< endl;
		this->robot->getRobotEnvironment()->printMap();
		this->gridOperations->setIterationCount(5);
		this->gridOperations->loopyBeliefPropagation();

		ROS_INFO("finishing processing");
//		this->gridOperations->getProcessedEnvironment()->printMap();
	};
};
