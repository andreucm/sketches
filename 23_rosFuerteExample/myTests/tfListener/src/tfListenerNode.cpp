#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <iostream>
#include "position3d.h"

int main(int argc, char** argv){
	
	ros::init(argc, argv, "robot_tf_listener");
	ros::NodeHandle n;
	tf::TransformListener tfListener;//(ros::Duration(10));
	tf::StampedTransform laserWRTbase;
	Cposition3d laserPose;

	//get laser pose with respect to the base link
 	tfListener.waitForTransform("base_link", "front_laser", ros::Time(0), ros::Duration(5.0));
 	tfListener.lookupTransform("base_link", "front_laser", ros::Time(0), laserWRTbase);
//	tfListener.waitForTransform("base_link", "front_laser", ros::Time::now(), ros::Duration(20.0));
//	tfListener.lookupTransform("base_link", "front_laser", ros::Time::now(), laserWRTbase);
 	tfListener.waitForTransform("base_link", "rear_laser", ros::Time(0), ros::Duration(5.0));
 	tfListener.lookupTransform("base_link", "rear_laser", ros::Time(0), laserWRTbase);

	std::cout	<< "laserWRTbase: ("; 
	std::cout 	<< laserWRTbase.getOrigin().x() << "," << laserWRTbase.getOrigin().y() << "," << laserWRTbase.getOrigin().z() << ","
			<< laserWRTbase.getRotation().getW() << "," << laserWRTbase.getRotation().getX() << "," << laserWRTbase.getRotation().getY() << "," << laserWRTbase.getRotation().getZ() << ")" << std::endl;

	laserPose.setXYZ(laserWRTbase.getOrigin().x(),laserWRTbase.getOrigin().y(),laserWRTbase.getOrigin().z());
	laserPose.setQuaternion(laserWRTbase.getRotation().getW(),laserWRTbase.getRotation().getX(),laserWRTbase.getRotation().getY(),laserWRTbase.getRotation().getZ());
	
	std::cout	<< "laserWRTbase: "; 
	laserPose.printPosition();

	//ros::spin();
}
