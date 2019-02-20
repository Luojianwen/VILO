file="/home/jianwen/VILO/KeyFrameTrajectory.txt"

if [ ! -f $file ]; then
	echo "File dose not exist"
else
	echo "File exists and has been removed before moving on"
	rm $file
fi

file="/home/jianwen/VILO/CameraTrajectory.txt"

if [ ! -f $file ]; then
	echo "File dose not exist"
else
	echo "File exists and has been removed before moving on"
	rm $file
fi

rosrun VILO RGBD /home/euclid/VILO/Vocabulary/ORBvoc.txt /home/euclid/VILO/Examples/ROS/VILO/rgbd_euclid.yaml
