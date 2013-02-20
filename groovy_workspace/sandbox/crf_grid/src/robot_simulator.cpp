#include "robot_simulator.h"

void publishEnvironmentTopic(Environment robotEnv) {
	/*float cell_width = 1;
	float cell_height = 1;
	std::vector<geometry_msgs::Point> points;
	geometry_msgs::Point point;
	nav_msgs::GridCells msg;
	ros::NodeHandle n;
	ros::Publisher chatter_pub = n.advertise<nav_msgs::GridCells>(GRIDCELLTOPIC, 1000);
	ros::Rate loop_rate(10);

	msg.cell_width = cell_width;
	msg.cell_height = cell_height;
	int count = 0;
	while (ros::ok())  {
		msg.header.frame_id = "my_frame";
		msg.header.stamp = ros::Time::now();

		ROS_INFO("count: %d", count);
		if (count < 10) {
			point.x = 20;
			point.y = 20;
			point.z = 0;

			points.push_back(point);
			point.x = 0;
			point.y = 0;
			point.z = 0;
			points.push_back(point);

			msg.cells = points;
		}
		else {
			cell_width = 1;
			cell_height = 1;
			msg.cell_width = cell_width;
			msg.cell_height = cell_height;

			point.x = 10;
			point.y = 10;
			point.z = 0;
			points.clear();
			points.push_back(point);				  
			msg.cells = points;
		}
	
	chatter_pub.publish(msg);

	ros::spinOnce();
	loop_rate.sleep();
	++count;
  }*/
};

int main(int argc, char **argv)
{
	Environment *env = new Environment();
//	Robot *robot = new Robot();
	Environment robotEnv;
	ros::init(argc, argv, "gridcell_sender");
	
	publishEnvironmentTopic(env);
	
	return 0;
};


