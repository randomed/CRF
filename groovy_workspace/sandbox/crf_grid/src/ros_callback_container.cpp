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
	Environment * groundTruth; //this is created off a random seed so that it is the same in the simulator, >>there is no actual data being passed from the simulator<<
	bool learnOnce; //tracks if the parameters for lbp have been learnt already
public:

	callbackContainer() {
		this->groundTruth = new Environment();	
		this->learnOnce = true;
	};
	void setRobot(Robot * robot) {
		this->robot = robot;
	}
	
	void setOccupancyGrid(occupancy_grid* gridOperations) { //this needs to be called after setRobot, so that an initial laser scan is processed and parameters can be learnt from it 
		this->gridOperations = gridOperations;
		int iterationCount = 3;
		this->gridOperations->setIterationCount(iterationCount);
	};
	void processLaserScan(const sensor_msgs::LaserScan::ConstPtr& msg) {
		float meanError;
		ROS_INFO("starting processing");
		this->robot->processLaserScan(msg);
		

		//learn parameters after at least one laser scan
		if (learnOnce) {
			this->gridOperations->learnParameters(this->groundTruth);
			this->learnOnce = false;
		}

//		this->robot->getRobotEnvironment()->generateTestMap();
//		cout << "unprocessed:"<< endl;
//		groundTruth->printMap();
//		this->robot->getRobotEnvironment()->printMap();
//		modifyEnvironment(this->robot->getRobotEnvironment());
		this->robot->getRobotEnvironment()->writeToFile("2beforelbp");
		this->gridOperations->loopyBeliefPropagation();
		
//		this->gridOperations->getProcessedEnvironment()->printMap();		
		this->gridOperations->getProcessedEnvironment()->writeToFile("afterlbp");
		ROS_INFO("finishing processing");
//		this->gridOperations->getProcessedEnvironment()->printMap();

		//section to compare lbp against matlab implementation
//		Environment * matlabEnv = new Environment(true);
//		matlabEnv->readFromFile("matlab_noisy");
//		occupancy_grid * matlabComparison = new occupancy_grid(matlabEnv, 2);
//		matlabComparison->loopyBeliefPropagation();
//		matlabComparison->getProcessedEnvironment()->writeToFile("matlabafterlbp");
		
		//section for testing methods
		Environment * testingEnv = new Environment(this->robot->getRobotEnvironment());
		occupancy_grid * tester = new occupancy_grid(testingEnv, 1);
//		testingEnv->printMap();
//		cout << "------------" << endl;
//		this->robot->getRobotEnvironment()->printMap();
//		tester->validation(groundTruth);
	};
};
