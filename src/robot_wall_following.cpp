#include "../include/robot_wall_following.h"

#define PI 3.14159265358979323846

using namespace std;

RobotWallFollowing::RobotWallFollowing(int argc,char **argv) {

    laser_topic_ = "robot0/laser_0";
	speeds_topic_ = "robot0/cmd_vel";

	subscriber_ = n_.subscribe(laser_topic_.c_str(), 1, &RobotWallFollowing::update, this);

	cmd_vel_pub_ = n_.advertise<geometry_msgs::Twist>(speeds_topic_.c_str(), 1);
}

RobotWallFollowing::~RobotWallFollowing() {
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
	float intendedDistance = 0.2;



    scan_ = msg;
	geometry_msgs::Twist cmd;
	char filename[] = "logs.txt";

	float minDistance = 100.0;
	float secondMinDistance = 100.0;

	float angleMin = 0.0;
	float angleSecondMin = 0.0;


	for (int i = 0; i < scan_.ranges.size(); i++) {

		float laserDistance = scan_.ranges[i];
		float laserAngle = scan_.angle_min + scan_.angle_increment * i;

		if (laserDistance < secondMinDistance) {
			if (laserDistance < minDistance) {
				secondMinDistance = minDistance;
				angleSecondMin = angleMin;
				minDistance = laserDistance;
				angleMin = laserAngle;
			} else {
				secondMinDistance = laserDistance;
				angleSecondMin = laserAngle;
			}
		}

	}


	float realMinDistance = 100.0;
	float realAng = 0.0;
	float angAux = 0.0;

	if(angleMin > angleSecondMin) {
		angAux = atan((secondMinDistance - minDistance*cos(scan_.angle_increment))/(minDistance*sin(scan_.angle_increment)));

		realAng = angleSecondMin + angAux;
		realMinDistance = secondMinDistance * cos(angAux);
	} else {
		angAux = atan((minDistance - secondMinDistance*cos(scan_.angle_increment))/(secondMinDistance*sin(scan_.angle_increment)));

		realAng = angleMin + angAux;
		realMinDistance = minDistance * cos(angAux);
	}

	//cases where only one laser hits the wall
	if(minDistance*2 < secondMinDistance) {
		realAng = angleMin;
		realMinDistance = minDistance;
	}



	const float GOAL_DISTANCE = 0.4;
	const float K = 15;

	// values for angular speed
	float alpha = PI/2 - abs(realAng);

	if (realMinDistance <= scan_.range_max)
	{
		cout << "******** WALKING *********" << endl;
		cout << "DISTANCE TO WALL: " << minDistance << endl;
		cout << "ALPHA: " << alpha << endl;

		cmd.linear.x = 0.5;

		//https://www.seas.upenn.edu/sunfest/docs/papers/12-bayer.pdf
		cmd.angular.z = (-K * (sin(alpha) - (realMinDistance - GOAL_DISTANCE))) * cmd.linear.x;
	}
	else
	{
		cmd.linear.x = 0.5;
		cmd.angular.z = 0.0;
	}

	cmd_vel_pub_.publish(cmd);

	char minDistanceStr[20];
	snprintf(minDistanceStr, 20, "%f", minDistance);
	char alphaStr[20];
	snprintf(alphaStr, 20, "%f", alpha);
	char linearStr[20];
	snprintf(linearStr, 20, "%f", cmd.linear.x);
	char angularStr[20];
	snprintf(angularStr, 20, "%f", cmd.angular.z);

	char log[400];
	strcpy(log, "=================\nwallDistance = ");
	strcat(log, minDistanceStr);
	strcat(log, "\nalpha = ");
	strcat(log, alphaStr);
	strcat(log, "\nSpeed:\n    linear = ");
	strcat(log, linearStr);
	strcat(log, "\n    angular = ");
	strcat(log, angularStr);
	strcat(log, "\n\n");
	writeToFile(filename, log);
	cout << log;


	
}