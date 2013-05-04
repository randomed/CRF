#include "Robot.cpp"
#include "occupancy_grid.cpp"
#include "ros/ros.h"
#include <time.h>
void modifyEnvironment(Environment * env) {
	//hardcoded features before lbp
	/*
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
	*/
	
	env->setMapping(3, 3, 1);	
	env->setMapping(4, 3, 1);	
	env->setMapping(4, 4, 1);	
	env->setMapping(3, 5, 1);	
	env->setMapping(2, 5, 1);	
	env->setMapping(4, 5, 1);	
	env->setMapping(3, 4, 1);	
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
		int sensitivityCounter = 0;
		vector<float> hiddenPotentials = vector<float>(3, 1); 
		vector<float> linkPotentials = vector<float>(2, 1); 
		ROS_INFO("starting processing");
		this->robot->processLaserScan(msg);
		
		//learn parameters after at least one laser scan
		if (learnOnce) {
//			this->gridOperations->learnParameters(this->groundTruth);
/*
			linkPotentials[0] = 1.1;
			linkPotentials[1] = 1.1;
			this->gridOperations->setLinkPotentials(linkPotentials);

			hiddenPotentials[0] = 7.8999;
			hiddenPotentials[1] = 2.4;
			hiddenPotentials[2] = 0;
			hiddenPotentials[3] = 12.3;
			this->gridOperations->setHiddenPotentials(hiddenPotentials);

			this->learnOnce = false;
*/
		}
		
		Environment * learningSetGroundTruth = new Environment(false);
		Environment * learningSetScan = new Environment(false);
		modifyEnvironment(learningSetGroundTruth);
		modifyEnvironment(learningSetScan);
		learningSetScan->setMapping(3, 4, 0.5);
		occupancy_grid * sandboxTest = new occupancy_grid(learningSetScan, 2);
		sandboxTest->learnParameters(learningSetGroundTruth);
//		learningSetScan->printMap();
		sandboxTest->loopyBeliefPropagation();
		sandboxTest->getProcessedEnvironment()->writeToFile("afterlbp");
		sandboxTest->getProcessedEnvironment()->printMap();
//		learningSetGroundTruth->printMap();
//		learningSetScan->writeToFile("2beforelbp");
		/*
		modifyEnvironment(this->robot->getRobotEnvironment());
		
		this->robot->getRobotEnvironment()->writeToFile("2beforelbp");
		cout << "grid before lbp mse = " << this->robot->getRobotEnvironment()->calculateError(this->groundTruth) << endl;
		this->robot->getRobotEnvironment()->printMap();

		this->gridOperations->loopyBeliefPropagation();
		
//		cout << "grid after lbp no learning mse = " << this->gridOperations->getProcessedEnvironment()->calculateError(this->groundTruth) << endl;
		cout << "grid after lbp with learning mse = " << this->gridOperations->getProcessedEnvironment()->calculateError(this->groundTruth) << endl;
		this->gridOperations->getProcessedEnvironment()->printMap();		
		this->gridOperations->getProcessedEnvironment()->writeToFile("afterlbp");
//		this->gridOperations->validation(this->groundTruth);
		*/
//		this->gridOperations->incrementHiddenPotential(-100);
		/*
		potentials[2] = 100;	
		for (sensitivityCounter = 0; sensitivityCounter < 10; sensitivityCounter ++) {
			this->gridOperations->setHiddenPotentials(potentials);
			this->gridOperations->validation(this->groundTruth);
			potentials[2] -= 10;
			potentials[0] += 20;
//			this->gridOperations->loopyBeliefPropagation();
//			this->gridOperations->getProcessedEnvironment()->printMap();
//			gridOperations->incrementHiddenPotential(20);	
		}
		*/
//		ROS_INFO("finishing processing");
//		this->gridOperations->getProcessedEnvironment()->printMap();
		
		//section to compare lbp against matlab implementation
		/*	
		Environment * matlabEnv = new Environment(true);
		Environment * matlabGroundTruth = new Environment(true);
		matlabGroundTruth->readFromFile("matlab_ground_truth");
		matlabEnv->readFromFile("matlab_noisy");
		occupancy_grid * matlabComparison = new occupancy_grid(matlabEnv, 2);

		cout << "no lbp mse = " << matlabEnv->calculateError(matlabGroundTruth) << endl;
//		matlabEnv->printMap();

		matlabComparison->loopyBeliefPropagation();
		cout << "lbp without learning mse = " << matlabComparison->getProcessedEnvironment()->calculateError(matlabGroundTruth) << endl;
//		matlabComparison->getProcessedEnvironment()->printMap();
//		matlabComparison->getProcessedEnvironment()->writeToFile("matlabafterlbp");
		matlabComparison->learnParameters(matlabGroundTruth);
//		matlabComparison->loopyBeliefPropagation();
//		cout << "lbp with learning mse = " << matlabComparison->getProcessedEnvironment()->calculateError(matlabGroundTruth) << endl;

//		matlabComparison->getProcessedEnvironment()->printMap();
		matlabComparison->getProcessedEnvironment()->writeToFile("matlabafterlbp");

//		matlabComparison->incrementHiddenPotential(-20);	
		for (sensitivityCounter = 0; sensitivityCounter < 5; sensitivityCounter ++) {
			matlabComparison->validation(matlabGroundTruth);
			matlabComparison->incrementHiddenPotential(20);	
		}
//		matlabComparison->validation(matlabGroundTruth);
		*/
		//section for testing methods
//		Environment * testingEnv = new Environment(this->robot->getRobotEnvironment());
//		occupancy_grid * tester = new occupancy_grid(testingEnv, 1);
//		testingEnv->printMap();
//		cout << "------------" << endl;
//		this->robot->getRobotEnvironment()->printMap();
//		tester->validation(groundTruth);
	};
};
