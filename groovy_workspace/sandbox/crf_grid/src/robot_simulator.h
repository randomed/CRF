#include "ros/ros.h"
#include "nav_msgs/GridCells.h"
#include "geometry_msgs/Point.h"
#include <vector>
#include <sstream>
#include "Robot.cpp"
#include "sensor_msgs/LaserScan.h"
#include "rviz_publish.cpp"
#include <boost/thread.hpp>
#define GRIDCELLTOPIC "draw_simulator"
#define ROBOTGROUNDTRUTHTOPIC "ground_truth_robot_position"
#define ROBOTLASERSCANTOPIC "laser_scan"
