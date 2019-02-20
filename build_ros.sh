echo "Building ROS nodes"

cd Examples/ROS/VILO
mkdir build
cd build
cmake .. -DROS_BUILD_TYPE=Release
make -j
