#include "ros/ros.h"
#include "nav_msgs/GridCells.h"
#include "geometry_msgs/Point.h"
#include <vector>
#include <sstream>
#include "Robot.h"
#define GRIDCELLTOPIC "testdraw"

void publishEnvironmentTopic(Environment robotEnv);
