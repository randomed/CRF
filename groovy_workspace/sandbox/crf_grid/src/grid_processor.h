#include "ros/ros.h"
#include "geometry_msgs/Point.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"
#include <vector>
#include "Robot.cpp"
#include "ros_callback_container.cpp"
#include "occupancy_grid.cpp"
#include "rviz_publish.cpp"
#include <boost/thread.hpp>

#define GRIDCELLTOPIC "draw_simulator"
#define ROBOTGROUNDTRUTHTOPIC "ground_truth_robot_position"
#define ROBOTLASERSCANTOPIC "laser_scan"
#define ROBOTTWISTTOPIC "twist"

//int receiveLaserScanThread(Robot * robot);
int receiveLaserScanThread(callbackContainer * container);
void sendTwistThread(Robot * robot); //receive external message from command line and then send twist?
