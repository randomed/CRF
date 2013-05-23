#include "robot_simulator.h"
#include "std_msgs/String.h"

void laserScanThread(Robot * robot, Environment * env) {
	ros::NodeHandle n;
	sensor_msgs::LaserScan scanMessage;

	ros::Publisher chatter_pub = n.advertise<sensor_msgs::LaserScan>(ROBOTLASERSCANTOPIC, 2);
//	ros::Publisher amcl_pub = n.advertise<sensor_msgs::LaserScan>("scan", 5);
	ros::Rate loop_rate(0.5);
	while (ros::ok())  {
		ROS_INFO("scanning..");
		robot->triggerSensors(env);
//		ROS_INFO("finished scan");
//		publishLaserScanTopic(robot->getLaserScan());
		scanMessage = robot->getLaserScan();
//		vector<float>::iterator it;
//		int ind = 0;
//		for (it = scanMessage.ranges.begin(); it != scanMessage.ranges.end(); it ++, ind ++) {
//
//			ROS_INFO("index: %d, distance %f", ind, scanMessage.ranges[ind]);
//		}
		scanMessage.header.frame_id = "laser_frame";
		scanMessage.header.stamp = ros::Time::now();

		chatter_pub.publish(scanMessage);
		//amcl_pub.publish(scanMessage);	
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

	ros::Publisher chatter_pub = n.advertise<nav_msgs::GridCells>(GRIDCELLTOPIC, 5);
	ros::Rate loop_rate(0.1);
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
		env->writeToFile("1groundtruth");
//		ROS_INFO("sending ground truth environment..");
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

	ros::Publisher chatter_pub = n.advertise<nav_msgs::GridCells>(ROBOTGROUNDTRUTHTOPIC, 5);
	ros::Rate loop_rate(1);
	gridCellsMessage.header.frame_id = "my_frame";


	while (ros::ok())  {
		points.clear();
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

void odometryThread(Robot * robot, Environment * env) {
/*	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe(ROBOTTWISTTOPIC, 5, &Robot::moveRobot, robot);
	ros::spin();
*/
	ros::NodeHandle n;
  ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 50);
  tf::TransformBroadcaster odom_broadcaster;

  double x = 0.0;
  double y = 0.0;
  double th = 0.0;

  double vx = 0.1;
  double vy = -0.1;
  double vth = 0.1;

  ros::Time current_time, last_time;
  current_time = ros::Time::now();
  last_time = ros::Time::now();

  ros::Rate r(1.0);
  while(n.ok()){

    ros::spinOnce();               // check for incoming messages
    current_time = ros::Time::now();

    //compute odometry in a typical way given the velocities of the robot
    double dt = (current_time - last_time).toSec();
    double delta_x = (vx * cos(th) - vy * sin(th)) * dt;
    double delta_y = (vx * sin(th) + vy * cos(th)) * dt;
    double delta_th = vth * dt;

    x += delta_x;
    y += delta_y;
    th += delta_th;

    //since all odometry is 6DOF we'll need a quaternion created from yaw
    geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);

    //first, we'll publish the transform over tf
    geometry_msgs::TransformStamped odom_trans;
    odom_trans.header.stamp = current_time;
    odom_trans.header.frame_id = "base_link";
    odom_trans.child_frame_id = "odom";

    odom_trans.transform.translation.x = x;
    odom_trans.transform.translation.y = y;
    odom_trans.transform.translation.z = 0.0;
    odom_trans.transform.rotation = odom_quat;

    //send the transform
    odom_broadcaster.sendTransform(odom_trans);

    //next, we'll publish the odometry message over ROS
    nav_msgs::Odometry odom;
    odom.header.stamp = current_time;
    odom.header.frame_id = "base_link";

    //set the position
    odom.pose.pose.position.x = x;
    odom.pose.pose.position.y = y;
    odom.pose.pose.position.z = 0.0;
    odom.pose.pose.orientation = odom_quat;

    //set the velocity
    odom.child_frame_id = "odom";
    odom.twist.twist.linear.x = vx;
    odom.twist.twist.linear.y = vy;
    odom.twist.twist.angular.z = vth;

    //publish the message
    odom_pub.publish(odom);

    last_time = current_time;
    r.sleep();
  }
};

void rmcl_getMap() {//move this to grid_processor later
	
	ros::NodeHandle n;
//	ros::ServiceClient map_service_client_ = n.serviceClient<nav_msgs::GetMap>("dynamic_map");
	nav_msgs::GetMap srv_map;
	vector<signed char>::iterator mapIt;
//	if (map_service_client_.call(srv_map)) {
	if (ros::service::call("dynamic_map", srv_map)) {
		  ROS_INFO("Map service called successfully");
		nav_msgs::OccupancyGrid map (srv_map.response.map);
		int i = 0;
		vector<signed char>::iterator mapIt = map.data.begin();
		Environment * env = new Environment();
		for (mapIt = map.data.begin(); mapIt != map.data.end(); mapIt ++) {
			
			i++;
			/*
//		for (i = 0; i < 10000; i++) {

//			convertedChar = atof((char ) map.data[i]);
//			cout << *mapIt;
//			if ((float) map.data[i] > -1) {	
//				printf("%f ", (float) map.data[i]);
//			}
			if ((float) *mapIt > 0) {
				cout << i % 4000 << ", " << floor(i / 4000) << " = " << (float) *mapIt << endl;	
//				printf("%f\n", (float) *mapIt);
			}
			*/
			if (i % 4000 > 1800 && i % 4000 < 2000 && floor(i / 4000) > 2000 && floor(i / 4000) < 2200) {
				if ((float ) *mapIt == -1) {
					env->setMapping((i % 4000) - 1800, floor(i / 4000) - 2000, 0.5);
				}
				else {
					env->setMapping((i % 4000) - 1800, floor(i / 4000) - 2000, (float) *mapIt);
				}
			}
		}
		env->writeToFile("partial_office");	
		ROS_INFO("done creating environment");
		
	}
	else
	{
		  ROS_ERROR("Failed to call map service for getMap");
//			 return;
	}
//	return;
};

void twistThread(Robot * robot) {
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe(ROBOTTWISTTOPIC, 2, &Robot::moveRobot, robot);
	ros::spin();
};

int main(int argc, char **argv)
{
	Environment *env = new Environment();
	Robot *robot = new Robot();
	robot->setRealEnv(env);
	ros::init(argc, argv, "simulation_sender");
	boost::thread t_laserScan(laserScanThread, robot, env);
	boost::thread t_groundTruth(publishEnvironmentThread, env);
	boost::thread t_robotPosition(publishRobotPosition, env);
	boost::thread t_odometry(odometryThread, robot, env);
	boost::thread t_twist(twistThread, robot);
//	boost::thread t_rmclMap(rmcl_getMap);
//	Environment robotEnv;
	
//	publishEnvironmentTopic(env, GRIDCELLTOPIC);
//	publishRobotPositionTopic(env, ROBOTGROUNDTRUTHTOPIC);
//	robot->triggerSensors(env);

	t_laserScan.join();
	t_groundTruth.join();
	t_robotPosition.join();
	t_odometry.join();
	t_twist.join();
//	t_rmclMap.join();
	return 0;
};


