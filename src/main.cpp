#include <ros/package.h>
#include "ros/ros.h"
#include "../include/robot_wall_following.h"


int main(int argc,char **argv)
{
  ros::init(argc, argv, "reactive", ros::init_options::AnonymousName);
  RobotWallFollowing robot(argc, argv);
  ros::spin();
  return 0;
}