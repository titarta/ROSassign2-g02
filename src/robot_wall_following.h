#ifndef ROBOT_WALL_FOLLOWING
#define ROBOT_WALL_FOLLOWING

#include <iostream>
#include <cstdlib>
#include <cmath>

#include <ros/package.h>
#include "ros/ros.h"

#include <stdlib.h>
#include <stdio.h>

#include <stdr_msgs/RobotIndexedVectorMsg.h>

#include <geometry_msgs/Pose2D.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>
#include <sensor_msgs/Range.h>

class RobotWallFollowing {
    private: 

        sensor_msgs::LaserScan scan_;
        ros::Subscriber subscriber_;
        ros::NodeHandle n_;
        std::string laser_topic_;
        std::string speeds_topic_;
        ros::Publisher cmd_vel_pub_;

    public:

        RobotWallFollowing(int argc,char **argv);
        ~RobotWallFollowing();
        update(const sensor_msgs::LaserScan& msg);

}

#endif