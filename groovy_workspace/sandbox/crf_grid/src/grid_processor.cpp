#include "grid_processor.h"
#include <ros/callback_queue.h>
int receiveLaserScanThread(callbackContainer * container) {
	ros::NodeHandle n;
//	ros::CallbackQueue q;
		ros::Subscriber sub = n.subscribe(ROBOTLASERSCANTOPIC, 2, &callbackContainer::processLaserScan, container);
	while (ros::ok()) {
//		ROS_INFO("here");
		ros::getGlobalCallbackQueue()->callOne(ros::WallDuration(1));
	}
//	q.callOne();
//	ros::spin();
	return 0;
};

/*
int receiveLaserScanThread(Robot * robot) {
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe(ROBOTLASERSCANTOPIC, 5, &Robot::processLaserScan, robot);
	ros::spin();
	return 0;
};
*/
void sendTwistThread(Robot * robot) {
	ros::NodeHandle n;
	geometry_msgs::Twist scanMessage;

	ros::Publisher chatter_pub = n.advertise<geometry_msgs::Twist>(ROBOTTWISTTOPIC, 2);
	ros::Rate loop_rate(1);
	scanMessage.linear.z = 0;
	
	int x = 0;
	while (ros::ok())  {
//		ROS_INFO("sending twist..");
//		x = 3;
		scanMessage.linear.x = (float) (x % 4);
		scanMessage.linear.y = 0;
		
		robot->forceMove(scanMessage.linear.x, scanMessage.linear.y);

		chatter_pub.publish(scanMessage);
		ros::spinOnce();
		loop_rate.sleep();
	}
	
};
void receiveOdometryThread(Robot * robot) {
	ros::NodeHandle n;
//	ros::Subscriber sub = n.subscribe(ROBOTLASERSCANTOPIC, 5, &Robot::processLaserScan, robot);
//	ros::spin();
};

int main(int argc, char **argv) {
	ros::init(argc, argv, "grid_processor_receiver");
	Robot * robot = new Robot();
	occupancy_grid * gridOperations = new occupancy_grid(robot->getRobotEnvironment(), 2);
	callbackContainer * container = new callbackContainer();
	container->setOccupancyGrid(gridOperations);
	container->setRobot(robot);
//	boost::thread t_laserScan(receiveLaserScanThread, robot);
	boost::thread t_laserScan(receiveLaserScanThread, container);
	boost::thread t_twist(sendTwistThread, robot);
	boost::thread t_odometry(receiveOdometryThread, robot);

	t_laserScan.join();
	t_twist.join();
	t_odometry.join();
	return 0;
};
