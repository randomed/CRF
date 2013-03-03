#include "grid_processor.h"

int receiveLaserScanThread(Robot * robot) {
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe(ROBOTLASERSCANTOPIC, 5, &Robot::processLaserScan, robot);
	ros::spin();
	return 0;
};

void sendTwistThread() {}; //receive external message from command line and then send twist?

void receiveOdometryThread(Robot * robot) {
	ros::NodeHandle n;
//	ros::Subscriber sub = n.subscribe(ROBOTLASERSCANTOPIC, 5, &Robot::processLaserScan, robot);
//	ros::spin();
};

int main(int argc, char **argv) {
	ros::init(argc, argv, "grid_processor_receiver");
	Robot *robot = new Robot();
	boost::thread t_laserScan(receiveLaserScanThread, robot);
	boost::thread t_twist(sendTwistThread);
	boost::thread t_odometry(receiveOdometryThread, robot);

	t_laserScan.join();
	t_twist.join();
	t_odometry.join();
	return 0;
};
