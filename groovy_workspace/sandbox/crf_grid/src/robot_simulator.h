#include "ros/ros.h"
#include "nav_msgs/GridCells.h"
#include "geometry_msgs/Point.h"
#include <vector>
#include <sstream>
#include "Robot.h"

#define GRIDCELLTOPIC "draw_simulator"
#define ROBOTGROUNDTRUTHTOPIC "ground_truth_robot_position"

void publishEnvironmentTopic(Environment robotEnv);
