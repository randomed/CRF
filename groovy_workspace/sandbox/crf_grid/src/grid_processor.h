#include "ros/ros.h"
#include "geometry_msgs/Point.h"
#include "sensor_msgs/LaserScan.h"
#include <vector>
#include "Robot.cpp"
#include "rviz_publish.cpp"

#define GRIDCELLTOPIC "draw_simulator"
#define ROBOTGROUNDTRUTHTOPIC "ground_truth_robot_position"
#define ROBOTLASERSCANTOPIC "laser_scan"
int receiveLaserScanTopic();
void chatterCallbacik(const sensor_msgs::LaserScan::ConstPtr& msg);
