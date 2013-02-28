#pragma once
#include <string>
#include "nav_msgs/GridCells.h"
/*
* Creates node and publishes topic, allows grid to be displayed in rviz.
*/
void publishEnvironmentTopic(Environment *robotEnv, std::string topic);
/*
* Utility function to put a marker of ground truth robot position
*/
void publishRobotPositionTopic(Environment *robotEnv, std::string topic);

void publishEnvironmentTopic(Environment *robotEnv, std::string topic) {
	int x, y, messageCount = 0;
	float cell_width = 1;
	float cell_height = 1;
	nav_msgs::GridCells gridCellsMessage;
	ros::NodeHandle n;
	std::vector<geometry_msgs::Point> points;
	geometry_msgs::Point current_point;

	gridCellsMessage.cell_width = cell_width;
	gridCellsMessage.cell_height = cell_height;
	current_point.z = 1;
	ros::Publisher chatter_pub = n.advertise<nav_msgs::GridCells>(topic, 1000);
	ros::Rate loop_rate(10);

	for (y = 0; y < robotEnv->getGridSizeVertical(); y++) {
		for (x = 0; x < robotEnv->getGridSizeHorizontal(); x++) {
			if (robotEnv->getMapping(x, y) > 0.6) {
				current_point.x = x;
				current_point.y = y;
				points.push_back(current_point);
				ROS_INFO("x = %d, y = %d", x, y);
			}
		}
	}
//	ROS_INFO("------------------");
	gridCellsMessage.header.frame_id = "my_frame";
	gridCellsMessage.header.stamp = ros::Time::now();
	gridCellsMessage.cells = points;

	while (ros::ok() && messageCount < 5)  {
		chatter_pub.publish(gridCellsMessage);
			
		ros::spinOnce();
		loop_rate.sleep();
		messageCount ++;
	}
};


void publishRobotPositionTopic(Environment *robotEnv, std::string topic) { //##rename this
	int messageCount = 0;
	float cell_width = 1;
	float cell_height = 1;
	nav_msgs::GridCells gridCellsMessage;
	ros::NodeHandle n;
	std::vector<geometry_msgs::Point> points;
	geometry_msgs::Point robot_point;
	gridCellsMessage.cell_width = cell_width;
	gridCellsMessage.cell_height = cell_height;
	robot_point.z = 1;
	ros::Publisher chatter_pub = n.advertise<nav_msgs::GridCells>(topic, 1000);
	ros::Rate loop_rate(10);

	robot_point.x = robotEnv->getRobotX();
	robot_point.y = robotEnv->getRobotY();
	points.push_back(robot_point);
	
	gridCellsMessage.header.frame_id = "my_frame";
	gridCellsMessage.header.stamp = ros::Time::now();
	gridCellsMessage.cells = points;

	while (ros::ok() && messageCount < 5)  {
		chatter_pub.publish(gridCellsMessage);
			
		ros::spinOnce();
		loop_rate.sleep();
		messageCount ++;
	}
};


