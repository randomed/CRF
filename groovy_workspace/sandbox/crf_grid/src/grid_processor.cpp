#include "grid_processor.h"

int receiveLaserScanTopic() {
	ros::NodeHandle n;
	Robot robot; //##change this after odometry is implemented
//	ros::Subscriber sub = n.subscribe(ROBOTLASERSCANTOPIC, 1000, chatterCallback);
	ros::Subscriber sub = n.subscribe(ROBOTLASERSCANTOPIC, 1000, &Robot::processLaserScan, &robot);
//	publishEnvironmentTopic(robot.getRobotEnvironment(), ROBOTGRIDVIEWTOPIC);
//	publishRobotGroundTruthTopic(robot->getRobotEnvironment(), ROBOT
//	publish
//	ROS_INFO("sending to gridCells..");
	ros::spin();
	return 0;
};

int main(int argc, char **argv) {
	ros::init(argc, argv, "grid_processor_receiver");
	receiveLaserScanTopic();
		
	return 0;
};
