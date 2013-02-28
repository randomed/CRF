#include "robot_simulator.h"
#include "std_msgs/String.h"
#include <sstream>
void chatterCallback(const std_msgs::String::ConstPtr& msg);
void publishLaserScanTopic(sensor_msgs::LaserScan laserScan) {//##delete this later
	int messageCount = 0;
	nav_msgs::GridCells gridCellsMessage;
	ros::NodeHandle n;
	sensor_msgs::LaserScan scanMessage;

	scanMessage = laserScan;
	scanMessage.header.frame_id = "my_frame";
	scanMessage.header.stamp = ros::Time::now();

	ros::Publisher chatter_pub = n.advertise<sensor_msgs::LaserScan>(ROBOTLASERSCANTOPIC, 5);
	ros::Rate loop_rate(10);

	while (ros::ok() && messageCount < 5)  {
		chatter_pub.publish(scanMessage);
			
		ros::spinOnce();
		loop_rate.sleep();
		messageCount ++;
	}
};

void laserScanThread(Robot * robot, Environment * env) {
	ros::NodeHandle n;
	sensor_msgs::LaserScan scanMessage;

	ros::Publisher chatter_pub = n.advertise<sensor_msgs::LaserScan>(ROBOTLASERSCANTOPIC, 1000);
	ros::Rate loop_rate(1);
	
	while (ros::ok())  {
		ROS_INFO("scanning..");
		robot->triggerSensors(env);
//		publishLaserScanTopic(robot->getLaserScan());
		scanMessage = robot->getLaserScan();

		vector<float>::iterator it;
		int ind = 0;
		for (it = scanMessage.ranges.begin(); it != scanMessage.ranges.end(); it ++, ind ++) {

			ROS_INFO("index: %d, distance %f", ind, scanMessage.ranges[ind]);
		}
		scanMessage.header.frame_id = "my_frame";
		scanMessage.header.stamp = ros::Time::now();

		chatter_pub.publish(scanMessage);
			
		ros::spinOnce();
		loop_rate.sleep();
	}
};

void publishEnvironmentThread(Environment * env) {
	int x, y;
	float cell_width = 1;
	float cell_height = 1;
	nav_msgs::GridCells gridCellsMessage;
	ros::NodeHandle n;
	std::vector<geometry_msgs::Point> points;
	geometry_msgs::Point current_point;
	gridCellsMessage.cell_width = cell_width;
	gridCellsMessage.cell_height = cell_height;
	current_point.z = 0;

	ros::Publisher chatter_pub = n.advertise<nav_msgs::GridCells>(GRIDCELLTOPIC, 1000);
	ros::Rate loop_rate(10);
	gridCellsMessage.header.frame_id = "my_frame";

	while (ros::ok())  {
		for (y = 0; y < env->getGridSizeVertical(); y++) {
			for (x = 0; x < env->getGridSizeHorizontal(); x++) {
				if (env->getMapping(x, y) > 0.6) {
					current_point.x = x;
					current_point.y = y;
					points.push_back(current_point);
				}
			}
		}
		gridCellsMessage.header.stamp = ros::Time::now();
		gridCellsMessage.cells = points;

		chatter_pub.publish(gridCellsMessage);
			
		ros::spinOnce();
		loop_rate.sleep();
	}
};

void publishRobotPosition(Environment *env) {
	float cell_width = 1;
	float cell_height = 1;
	nav_msgs::GridCells gridCellsMessage;
	ros::NodeHandle n;
	std::vector<geometry_msgs::Point> points;
	geometry_msgs::Point robot_point;
	gridCellsMessage.cell_width = cell_width;
	gridCellsMessage.cell_height = cell_height;
	robot_point.z = 0;

	ros::Publisher chatter_pub = n.advertise<nav_msgs::GridCells>(ROBOTGROUNDTRUTHTOPIC, 1000);
	ros::Rate loop_rate(10);
	gridCellsMessage.header.frame_id = "my_frame";

	while (ros::ok())  {
		robot_point.x = env->getRobotX();
		robot_point.y = env->getRobotY();
		points.push_back(robot_point);
		
		gridCellsMessage.header.stamp = ros::Time::now();
		gridCellsMessage.cells = points;

		chatter_pub.publish(gridCellsMessage);
			
		ros::spinOnce();
		loop_rate.sleep();
	}


};

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
//ROS_INFO("here2");
	ROS_INFO("I heard: [%s]", msg->data.c_str());
}

int main(int argc, char **argv)
{

	Environment *env = new Environment();
	Robot *robot = new Robot();
	ros::init(argc, argv, "simulation_sender");
	boost::thread t_laserScan(laserScanThread, robot, env);
	boost::thread t_groundTruth(publishEnvironmentThread, env);
	boost::thread t_robotPosition(publishRobotPosition, env);
	Environment robotEnv;
	
//	publishEnvironmentTopic(env, GRIDCELLTOPIC);
//	publishRobotPositionTopic(env, ROBOTGROUNDTRUTHTOPIC);
//	robot->triggerSensors(env);

	t_laserScan.join();
	t_groundTruth.join();
	t_robotPosition.join();	
	return 0;
};


