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

void RobotWallFollowing::update(const sensor_msgs::LaserScan &msg) {
	const float GOAL_DISTANCE = 0.2; //distancia linha a parede
	const float K = 10; //valor para dar tune a velocidade angular
	const float F = 0.5; //valor para dar tune a correcao da velocidade angular usando o laser da frente
	const float frontAngle = msg.angle_increment/2 + 0.01; //angulos para saber quais os lasers da frente
	const float velocity = 0.5; //velocidade do robo
	const float maxAngVelocity = 2.5; //velocidade angular maxima


    scan_ = msg;//msg tens a informacao dos lasers, ver sensor_msgs:LaserScan api para mais informacao
	geometry_msgs::Twist cmd;
	char filename[] = "logs.txt";

	float minDistance = 100.0; //distancias que depois vao ter o minimo
	float secondMinDistance = 100.0;
	float frontDistance = 100.0;

	float angleMin = 0.0;//angulos das distancias minimas
	float angleSecondMin = 0.0;


	for (int i = 0; i < scan_.ranges.size(); i++) {

		float laserDistance = scan_.ranges[i];
		float laserAngle = scan_.angle_min + scan_.angle_increment * i;

		if(laserAngle <= frontAngle && laserAngle >= -frontAngle) {
			if (laserDistance <= frontDistance) {
				frontDistance = laserDistance;
			}
		}

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

	float frontDeviation = 0.0; //valor que vai ser adicionado a formula para as curvas apertadas serem mais smooth

	if (frontDistance < 2*GOAL_DISTANCE) {
		frontDeviation = (PI*GOAL_DISTANCE)/(2*frontDistance);
	}


	float realMinDistance = 100.0; //distancia robo a parede
	float realAng = 0.0; //angulo da reta perpendicular a parede
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


	// values for angular speed
	float alpha = PI/2 - abs(realAng);

	if (realMinDistance <= scan_.range_max)
	{
		cmd.linear.x = velocity;

		//https://www.seas.upenn.edu/sunfest/docs/papers/12-bayer.pdf
		//big formula that does all the work
		cmd.angular.z = (-K * (sin(alpha) - (realMinDistance - GOAL_DISTANCE) + F*frontDeviation)) * cmd.linear.x;

		if (cmd.angular.z > maxAngVelocity) { //limitar a velocidade angular
			cmd.angular.z = maxAngVelocity;
		}
	}
	else
	{
		cmd.linear.x = velocity;
		cmd.angular.z = velocity*velocity/2; //robo anda a roda 
	}

	cmd_vel_pub_.publish(cmd);

	//prints
	char minDistanceStr[20];
	snprintf(minDistanceStr, 20, "%f", realMinDistance);
	char alphaStr[20];
	snprintf(alphaStr, 20, "%f", alpha);
	char linearStr[20];
	snprintf(linearStr, 20, "%f", cmd.linear.x);
	char angularStr[20];
	snprintf(angularStr, 20, "%f", cmd.angular.z);
	char frontDistanceStr[20];
	snprintf(frontDistanceStr, 20, "%f", frontDeviation);

	char log[400];
	strcpy(log, "=================\nwallDistance = ");
	strcat(log, minDistanceStr);
	strcat(log, "\nalpha = ");
	strcat(log, alphaStr);
	strcat(log, "\nfrontDeviation = ");
	strcat(log, frontDistanceStr);
	strcat(log, "\nSpeed:\n    linear = ");
	strcat(log, linearStr);
	strcat(log, "\n    angular = ");
	strcat(log, angularStr);
	strcat(log, "\n\n");
	cout << log;
}