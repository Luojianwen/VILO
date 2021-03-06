/**
* This file is part of VILO.
*
* Copyright (C) 2019 Jianwen Luo <luojianwen1123 at gmail dot com> (Southern University of Science and Technology)
* For more information see <https://github.com/Luojianwen/VILO>
*
* VILO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* VILO is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with VILO. If not, see <http://www.gnu.org/licenses/>.
*/


#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>

#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <std_msgs/String.h>
#include <geometry_msgs/PoseStamped.h>
#include <sensor_msgs/Imu.h>

#include<opencv2/core/core.hpp>

#include"../../../include/System.h"

using namespace std;

class ImageGrabber
{
public:
    ImageGrabber(ORB_SLAM2::System* pSLAM):mpSLAM(pSLAM){}

    void GrabRGBD(const sensor_msgs::ImageConstPtr& msgRGB,const sensor_msgs::ImageConstPtr& msgD);
    ORB_SLAM2::System* mpSLAM;
    ros::Publisher pub;
    //int cnt; // in case we need to show count number, for debug use.
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "RGBD");
    ros::start();

    if(argc != 3)
    {
        cerr << endl << "Usage: rosrun ORB_SLAM2 RGBD path_to_vocabulary path_to_settings" << endl;        
        ros::shutdown();
        return 1;
    }    

    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    ORB_SLAM2::System SLAM(argv[1],argv[2],ORB_SLAM2::System::RGBD,false);

    ImageGrabber igb(&SLAM);

    ros::NodeHandle nh;

    message_filters::Subscriber<sensor_msgs::Image> rgb_sub(nh, "/camera/color/image_raw", 1);
    message_filters::Subscriber<sensor_msgs::Image> depth_sub(nh, "/camera/depth/image_raw", 1);
    typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image, sensor_msgs::Image> sync_pol;
    message_filters::Synchronizer<sync_pol> sync(sync_pol(10), rgb_sub,depth_sub);
    sync.registerCallback(boost::bind(&ImageGrabber::GrabRGBD,&igb,_1,_2));
     
    igb.pub = nh.advertise<geometry_msgs::PoseStamped>("vo", 2);
    //igb.cnt = 0;

    ros::spin();

    std::cout<<"Starting shutdown ..."<<std::endl;
    // Stop all threads
    SLAM.Shutdown();
    std::cout<<"Saving trajectory ..."<<std::endl;
    // Save camera trajectory
    SLAM.SaveKeyFrameTrajectoryTUM("KeyFrameTrajectory.txt");

    ros::shutdown();

    return 0;
}

void ImageGrabber::GrabRGBD(const sensor_msgs::ImageConstPtr& msgRGB,const sensor_msgs::ImageConstPtr& msgD)
{
    // Copy the ros image message to cv::Mat.
    cv_bridge::CvImageConstPtr cv_ptrRGB;
    try
    {
        cv_ptrRGB = cv_bridge::toCvShare(msgRGB);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

    cv_bridge::CvImageConstPtr cv_ptrD;
    try
    {
        cv_ptrD = cv_bridge::toCvShare(msgD);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

/*
int d_h, d_w,rgb_h, rgb_w, d_channels, rgb_channels;
d_h   = cv_ptrD->image.rows;
d_w   = cv_ptrD->image.cols;
rgb_h = cv_ptrRGB->image.rows;
rgb_w = cv_ptrRGB->image.cols;
d_channels = cv_ptrD->image.channels();
rgb_channels = cv_ptrRGB->image.channels();
int d_type = cv_ptrD->image.type();
int cv_32f = CV_32F;
std::cout<<"depth: "<<d_h<<" by "<<d_w<<", channels: "<<
d_channels<<" type: "<< d_type <<"  rgb: "<<rgb_h<<" by "<<rgb_w<<", channels: "<<
rgb_channels<<" type: "<<cv_32f<<std::endl;
*/

    cv::Mat M;
//#ifdef COMPILEDWITHC11
        //std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
//#else
//        std::chrono::monotonic_clock::time_point t1 = std::chrono::monotonic_clock::now();
//#endif
    M = mpSLAM->TrackRGBD(cv_ptrRGB->image,cv_ptrD->image,cv_ptrRGB->header.stamp.toSec());
//#ifdef COMPILEDWITHC11
        //std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
//#else
//        std::chrono::monotonic_clock::time_point t2 = std::chrono::monotonic_clock::now();
//#endif

    //double ttrack= std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1).count();
    //cout << "Time consumed : " << ttrack * 1e3 << "ms" <<endl;
    if(!M.empty())
    {
        geometry_msgs::PoseStamped CameraPose;
    CameraPose.header.stamp = cv_ptrRGB->header.stamp;
    CameraPose.header.seq = cv_ptrRGB->header.seq;
    CameraPose.header.frame_id = "vo_frame";
    CameraPose.pose.position.x = M.at<float>(0,3);
    CameraPose.pose.position.y = M.at<float>(1,3);
    CameraPose.pose.position.z = M.at<float>(2,3);
    CameraPose.pose.orientation.x = 0;
    CameraPose.pose.orientation.y = 0;
    CameraPose.pose.orientation.z = 0;
    CameraPose.pose.orientation.w = 10;
    
    std::cout<<"stamp seq:  "<<cv_ptrRGB->header.seq<<std::endl;
    std::cout<<"time: "<<cv_ptrRGB->header.stamp.sec<<std::endl;
    pub.publish(CameraPose);
    }
    else{
        std::cout<<"Tracking failed ... "<<std::endl;
    }

/*
    std::cout<<"z--->"<<CameraPose.pose.position.z<<std::endl;
    pub.publish(CameraPose);

    std_msgs::String msg;
    std::stringstream ss;
    ss<<"Hi James"<<cnt;
    msg.data = ss.str();
*/
}