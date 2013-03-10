#include "ros/ros.h"
#include "geometry_msgs/Point.h"
#include "sensor_msgs/LaserScan.h"
#include <vector>
#include "Robot.cpp"
#include "ros_callback_container.cpp"
#include "occupancy_grid.cpp"
#include "rviz_publish.cpp"
#include <boost/thread.hpp>

#define GRIDCELLTOPIC "draw_simulator"
#define ROBOTGROUNDTRUTHTOPIC "ground_truth_robot_position"
#define ROBOTLASERSCANTOPIC "laser_scan"

//int receiveLaserScanThread(Robot * robot);
int receiveLaserScanThread(callbackContainer * container);
