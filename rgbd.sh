rm ./Examples/RGB-D/associations.txt
python2 ./Examples/RGB-D/associate.py /media/euclid/JianwenLuo/0-dataset/rgbd_dataset_freiburg1_floor/rgb.txt /media/euclid/JianwenLuo/0-dataset/rgbd_dataset_freiburg1_floor/depth.txt > ./Examples/RGB-D/associations.txt
rm CameraTrajectory.txt KeyFrameTrajectory.txt
./Examples/RGB-D/rgbd_tum Vocabulary/ORBvoc.txt Examples/RGB-D/TUM2.yaml /media/euclid/JianwenLuo/0-dataset/rgbd_dataset_freiburg1_floor /home/euclid/ORB_SLAM2/Examples/RGB-D/associations.txt