#include "robot_simulator.h"

void publishLaserScanTopic(sensor_msgs::LaserScan laserScan) {
	int messageCount = 0;
	nav_msgs::GridCells gridCellsMessage;
	ros::NodeHandle n;
	sensor_msgs::LaserScan scanMessage;

	scanMessage = laserScan;
	scanMessage.header.frame_id = "my_frame";
	scanMessage.header.stamp = ros::Time::now();

	ros::Publisher chatter_pub = n.advertise<sensor_msgs::LaserScan>(ROBOTLASERSCANTOPIC, 1000);
	ros::Rate loop_rate(10);

	while (ros::ok() && messageCount < 5)  {
		chatter_pub.publish(scanMessage);
			
		ros::spinOnce();
		loop_rate.sleep();
		ROS_INFO("sending..");
		messageCount ++;
	}
};

int main(int argc, char **argv)
{
	Environment *env = new Environment();
	Robot *robot = new Robot();
	Environment robotEnv;
	ros::init(argc, argv, "simulation_sender");
	
	publishEnvironmentTopic(env, GRIDCELLTOPIC);
	publishRobotGroundTruthTopic(env, ROBOTGROUNDTRUTHTOPIC);
	robot->triggerSensors(env);
//	robotEnv = robot->getRobotEnvironment();	
	
	publishLaserScanTopic(robot->getLaserScan());	
		
	return 0;
};


