file="/home/jianwen/ORB_SLAM2/KeyFrameTrajectory.txt"

if [ ! -f $file ]; then
	echo "File dose not exist"
else
	echo "File exists and has been removed before moving on"
	rm $file
fi

file="/home/jianwen/ORB_SLAM2/CameraTrajectory.txt"

if [ ! -f $file ]; then
	echo "File dose not exist"
else
	echo "File exists and has been removed before moving on"
	rm $file
fi

rosrun ORB_SLAM2 RGBD /home/euclid/ORB_SLAM2/Vocabulary/ORBvoc.txt /home/euclid/ORB_SLAM2/Examples/ROS/ORB_SLAM2/rgbd_euclid.yaml