#include "ros/ros.h"
#include <math.h>
#include "geometry_msgs/Pose2D.h"										// to get desired position command
#include "turtlesim/Pose.h"			
#include <iostream>
#include <fstream>								                        // to read current position from points.txt
#include "geometry_msgs/Twist.h"										// to send velocity command
using namespace std;
// Function declarations
void ComPoseCallback(float a[][2] , int i);
void CurPoseCallback(const turtlesim::Pose::ConstPtr& msg);
float GetErrorLin(turtlesim::Pose curpose, geometry_msgs::Pose2D despose);
float GetErrorAng(turtlesim::Pose curpose, geometry_msgs::Pose2D despose);
int number =0;
// Global variables
bool STOP = true;														// to hold stop flag, wait till first command given
turtlesim::Pose CurPose;												// to hold current pose
geometry_msgs::Pose2D DesPose;											// variable to hold desired pose

int main(int argc, char **argv)
{
	
	ros::init(argc, argv, "TurtlesimPositionController_pubsub");		// connect to roscore
	ros::NodeHandle n;													// node object

	// register sub to get current position/pose
	ros::Subscriber CurPose_sub = n.subscribe("/turtle1/pose", 5, CurPoseCallback);
	// register pub to send twist velocity (cmd_vel)
	ros::Publisher Twist_pub = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 100);
	float a[500][2];	
	ifstream fin;
	fin.open("points.txt");
	while(fin.eof()==0)
	{
		fin>>a[number][0]>>a[number][1];
		number++;
		cout<<number<<endl;
	}
	cout<<a[0][0]<<"  "<<a[0][1];
	

	ros::Rate loop_rate(10);											// freq to run loops in (10 Hz)
	float ErrorLin = 0;													// holds linear error
	float ErrorAng = 0;													// holds angular error
	geometry_msgs::Twist CmdVel;										// holds command velocity to publish
	
	int i=0;
	ROS_INFO("Ready to send position commands");						// let user know we are ready and good
	while (ros::ok() && n.ok() )										// while ros and the node are ok
	{
		ros::spinOnce();
		if(i==(number+1))
			break;
		ComPoseCallback(a,i);
		
		if (STOP == false)												// and no stop command
		{
			ErrorLin = GetErrorLin(CurPose, DesPose);					// find linear error
			ErrorAng = GetErrorAng(CurPose, DesPose);					// find angular error
			cout<<"Error linear: "<<ErrorLin<< "Error angular:"<< ErrorAng<<"\n";
			
			CmdVel.linear.x = 0.2 * ErrorLin;							// multiplying by linear P for control signal
			CmdVel.angular.z = 0.5 * ErrorAng;							// multiplying by linear P for control signal
			Twist_pub.publish(CmdVel);									// publish to move turtle
		}
		else
		{
			cout<<"waiting......."<<"\n";								// waiting for first command
		}
		loop_rate.sleep();												// sleep to maintain loop rate
		if(fabs(ErrorLin)<0.1)
			i++;
	}
}


// call back to send new desired Pose msgs
void ComPoseCallback(float a[][2], int i)			
{
	STOP = false;														// start loop
	DesPose.x = a[i][0];													// copy msg to varible to use elsewhere
	DesPose.y = a[i][1];
	return;
}

// call back to send new current Pose msgs
void CurPoseCallback(const turtlesim::Pose::ConstPtr& msg)			
{
	CurPose.x = msg->x;
	CurPose.y = msg->y;
	CurPose.theta = msg->theta;											// copy msg to varible to use elsewhere
	return;
}

// function to get angular error between facing direction of the turtle and direction to desired pose
float GetErrorAng(turtlesim::Pose curpose, geometry_msgs::Pose2D despose)
{
	// create error vector
	float Ex = despose.x - curpose.x;									// Error X. X component 
	float Ey = despose.y - curpose.y;									// Error Y. Y component 
	
	// get desired angle
	float dest = atan2f(Ey, Ex); 										// use float version to get arc tangent
	
	// get angle error
	float Et = dest - curpose.theta;
	
	//~ ROS_INFO("Ex: %f, Ey: %f, Et: %f", Ex, Ey, Et);
	return Et;
}

// function to get linear error from the turtles perspective. Error only along turtle X axis
float GetErrorLin(turtlesim::Pose curpose, geometry_msgs::Pose2D despose)
{
	// create error vector
	float Ex = despose.x - curpose.x;									// Error X. X component
	float Ey = despose.y - curpose.y;									// Error Y. Y component 
	float Et = GetErrorAng(curpose, despose);							// get angle between vectors
	
	// project error onto turtle x axis
	//~ float Etx =  pow( pow(Ex,2.0) + pow(Ey,2.0), 0.5 )*cos(Et);
	float Etx = hypotf(Ex, Ey)*cos(Et); // improved c function
	
	return Etx;
}