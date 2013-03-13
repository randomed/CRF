#include "ros/ros.h"
#include "nav_msgs/GridCells.h"
#include "geometry_msgs/Point.h"
#include <vector>
#include <sstream>
#include "Robot.cpp"
#include "sensor_msgs/LaserScan.h"
#include <boost/thread.hpp>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <nav_msgs/OccupancyGrid.h>
#include <nav_msgs/GetMap.h>
#define GRIDCELLTOPIC "draw_simulator"
#define ROBOTGROUNDTRUTHTOPIC "ground_truth_robot_position"
#define ROBOTLASERSCANTOPIC "laser_scan"
#define ROBOTTWISTTOPIC "twist"


void rmcl_getMap();//move this to grid_processor later
