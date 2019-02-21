echo "Uncompress vocabulary ..."

cd Vocabulary
tar -xf ORBvoc.txt.tar.gz
cd ..

echo "Configuring and building VILO ..."

mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j

cd ..

echo "Building ROS nodes"

cd Examples/ROS/VILO
mkdir build
cd build
cmake .. -DROS_BUILD_TYPE=Release
make -j
