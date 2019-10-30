#include "../include/robot_wall_following.h"

using namespace std;

RobotWallFollowing::RobotWallFollowing(int argc,char **argv) {

    laser_topic_ = "robot0/laser_0";
	speeds_topic_ = "robot0/cmd_vel";

	subscriber_ = n_.subscribe(laser_topic_.c_str(), 1, &RobotWallFollowing::update, this);

	cmd_vel_pub_ = n_.advertise<geometry_msgs::Twist>(speeds_topic_.c_str(), 1);
}

void writeToFile(char* filename, char* str)
{
	FILE *f = fopen(filename, "a");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}

	/* print some text */
	fprintf(f, "%s", str);

	fclose(f);
}

void RobotWallFollowing::update(const sensor_msgs::LaserScan &msg) {

    scan_ = msg;
	geometry_msgs::Twist cmd;
	char filename[] = "logs.txt";




    cmd.linear.x = 0.5;
	cmd.angular.z = 0.0;

    cmd_vel_pub_.publish(cmd);
}