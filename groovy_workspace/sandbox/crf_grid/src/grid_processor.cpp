#include "grid_processor.h"

int receiveLaserScanTopic() {
	ros::NodeHandle n;
	Robot robot; //##change this after odometry is implemented
	ros::Subscriber sub = n.subscribe(ROBOTLASERSCANTOPIC, 5, &Robot::processLaserScan, &robot);
	ros::spin();
	return 0;
};

int main(int argc, char **argv) {
	ros::init(argc, argv, "grid_processor_receiver");
	receiveLaserScanTopic();
	return 0;
};
