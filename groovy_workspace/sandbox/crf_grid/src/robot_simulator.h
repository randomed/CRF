#include "ros/ros.h"
#include "nav_msgs/GridCells.h"
#include "geometry_msgs/Point.h"
#include <vector>
#include <sstream>
#include "Robot.cpp"
#include "sensor_msgs/LaserScan.h"

#define GRIDCELLTOPIC "draw_simulator"
#define ROBOTGROUNDTRUTHTOPIC "ground_truth_robot_position"
#define ROBOTLASERSCANTOPIC "laser_scan"
void publishEnvironmentTopic(Environment robotEnv);
void publishLaserScanTopic(sensor_msgs::LaserScan laserScan);
