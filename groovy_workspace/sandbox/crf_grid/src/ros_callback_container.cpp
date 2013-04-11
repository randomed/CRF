#include "Robot.cpp"
#include "occupancy_grid.cpp"
#include "ros/ros.h"
#include <time.h>
void modifyEnvironment(Environment * env) {
	//hardcoded features before lbp
	int x, y;
	stringstream ss;
	srand(time(NULL));
	
	//populate randomly
	
	int randomx, randomy;
	y = 0;
	x = 0;
	for (int i = 0; i < 2; i ++) {
		x = rand() % (env->getGridSizeHorizontal() - 1);
		//randomx = 1 + x + (rand() % (this->gridSizeHorizontal - x));
		randomx =  1 + x + rand() % 10;
		for (;x < randomx; x ++) {
			y = rand() % (env->getGridSizeVertical() - 1);
			//randomy = 1 + y + (rand() % (this->gridSizeVertical - y));
			randomy =  1 + y + rand() % 10;
			for (;y < randomy; y ++) {
				env->setMapping(x, y, 1);
				env->addHashedMapping(x, y);
			}
		}
	}	
};
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
		int iterationCount = 2;

		ROS_INFO("starting processing");
		this->robot->processLaserScan(msg);
//		this->robot->getRobotEnvironment()->generateTestMap();
//		cout << "unprocessed:"<< endl;
		this->robot->getRobotEnvironment()->printMap();
//		modifyEnvironment(this->robot->getRobotEnvironment());
		this->robot->getRobotEnvironment()->writeToFile("2beforelbp");
		this->gridOperations->setIterationCount(iterationCount);
		this->gridOperations->loopyBeliefPropagation();
		
//		this->gridOperations->getProcessedEnvironment()->printMap();		
		this->gridOperations->getProcessedEnvironment()->writeToFile("afterlbp");
		ROS_INFO("finishing processing");
//		this->gridOperations->getProcessedEnvironment()->printMap();

		//section to compare lbp against matlab implementation
		Environment * matlabEnv = new Environment(true);
		matlabEnv->readFromFile("matlab_noisy");
		occupancy_grid * matlabComparison = new occupancy_grid(matlabEnv, 2);
		matlabComparison->loopyBeliefPropagation();
		matlabComparison->getProcessedEnvironment()->writeToFile("matlabafterlbp");
	};
};
