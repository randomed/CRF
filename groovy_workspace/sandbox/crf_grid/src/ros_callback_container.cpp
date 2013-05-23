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
	
	env->setMapping(2, 2, 1);	
	env->setMapping(3, 2, 1);	
	env->setMapping(4, 2, 1);	
	env->setMapping(2, 3, 1);	
	env->setMapping(3, 3, 1);	
	env->setMapping(4, 3, 1);	
	env->setMapping(2, 4, 1);	
	env->setMapping(3, 4, 1);	
	env->setMapping(4, 4, 1);	
	
	env->setMapping(9, 2, 1);	
	env->setMapping(10, 2, 1);	
	env->setMapping(11, 2, 1);	
	env->setMapping(9, 3, 1);	
	env->setMapping(10, 3, 1);	
	env->setMapping(11, 3, 1);	
	env->setMapping(9, 4, 1);	
	env->setMapping(10, 4, 1);	
	env->setMapping(11, 4, 1);
	
	env->setMapping(2, 9, 1);	
	env->setMapping(3, 9, 1);	
	env->setMapping(4, 9, 1);	
	env->setMapping(2, 10, 1);	
	env->setMapping(3, 10, 1);	
	env->setMapping(4, 10, 1);	
	env->setMapping(2, 11, 1);	
	env->setMapping(3, 11, 1);	
	env->setMapping(4, 11, 1);	
	
	env->setMapping(9, 9, 1);	
	env->setMapping(10,9, 1);	
	env->setMapping(11,9, 1);	
	env->setMapping(9, 10, 1);	
	env->setMapping(10,10, 1);	
	env->setMapping(11,10, 1);	
	env->setMapping(9, 11, 1);	
	env->setMapping(10, 11, 1);	
	env->setMapping(11, 11, 1);		
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
			linkPotentials[1] = 1.3;
			this->gridOperations->setLinkPotentials(linkPotentials);

			hiddenPotentials[0] = 10.3;
			hiddenPotentials[1] = 1.3;
			hiddenPotentials[2] = 0.9;
			hiddenPotentials[3] = 10.2;
			this->gridOperations->setHiddenPotentials(hiddenPotentials);

			this->learnOnce = false;
			*/
		}
		/*
		Environment * learningSetGroundTruth = new Environment(false);
		Environment * learningSetScan = new Environment(false);
		learningSetGroundTruth->readFromFile("box unfilled learning ground truth");
		learningSetScan->readFromFile("box learning scan");

		learningSetScan->writeToFile("box learning scan");
		occupancy_grid * learningBox = new occupancy_grid(learningSetScan, 2);
		learningBox->learnParameters(learningSetGroundTruth);
//		learningSetScan->printMap();
		learningBox->loopyBeliefPropagation();
		learningBox->getProcessedEnvironment()->writeToFile("afterlbp");
//		learningBox->getProcessedEnvironment()->printMap();
//		learningSetGroundTruth->printMap();
//		learningSetScan->writeToFile("2beforelbp");

		Environment * testingSetGroundTruth = new Environment(false);
		Environment * testingSetScan = new Environment(false);
		testingSetGroundTruth->readFromFile("box unfilled");
		testingSetScan->readFromFile("box scan");
		occupancy_grid * testingBox  = new occupancy_grid(testingSetScan, 2);
		
		cout << "grid before lbp mse = " << testingBox->getProcessedEnvironment()->calculateError(testingSetGroundTruth) << endl;
	
	
		testingBox->loopyBeliefPropagation();
		cout << "grid after lbp without learning mse = " << testingBox->getProcessedEnvironment()->calculateError(testingSetGroundTruth) << endl;
		testingBox->getProcessedEnvironment()->writeToFile("box after lbp no learning");
//		testingBox->validation(testingSetGroundTruth);
		hiddenPotentials = learningBox->getHiddenPotentials();
		hiddenPotentials[2] = 2;	
//		testingBox->setHiddenPotentials(learningBox->getHiddenPotentials());	
		testingBox->setHiddenPotentials(hiddenPotentials);	
		testingBox->setLinkPotentials(learningBox->getLinkPotentials());	

		testingBox->learnParameters(testingSetGroundTruth);
		testingBox->loopyBeliefPropagation();
		cout << "grid after lbp with learning mse = " << testingBox->getProcessedEnvironment()->calculateError(testingSetGroundTruth) << endl;
		testingBox->getProcessedEnvironment()->writeToFile("box after lbp with learning");
		testingBox->validation(testingSetGroundTrut/h);	
		*/
		
		/*	
		this->robot->getRobotEnvironment()->writeToFile("2beforelbp");
		cout << "grid before lbp mse = " << this->robot->getRobotEnvironment()->calculateError(this->groundTruth) << endl;
//		this->robot->getRobotEnvironment()->printMap();

		this->gridOperations->loopyBeliefPropagation();
		
//		cout << "grid after lbp no learning mse = " << this->gridOperations->getProcessedEnvironment()->calculateError(this->groundTruth) << endl;
		cout << "grid after lbp with learning mse = " << this->gridOperations->getProcessedEnvironment()->calculateError(this->groundTruth) << endl;
//		this->gridOperations->getProcessedEnvironment()->printMap();		
		this->gridOperations->getProcessedEnvironment()->writeToFile("afterlbp");
		this->gridOperations->validation(this->groundTruth);
		
//		this->gridOperations->incrementHiddenPotential(-100);
		*/
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

		//real life environment
		Environment * real = new Environment(false);
		real->readFromFile("partial_office");
		occupancy_grid * officeMap = new occupancy_grid(real, 2);
		officeMap->loopyBeliefPropagation();
		officeMap->getProcessedEnvironment()->writeToFile("real_after_lbp");
//		officeMap->validation(real);
		ROS_INFO("done lbp");
		
		
	};
};
