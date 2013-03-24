cmake_minimum_required(VERSION 2.4.6)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
#set(ROS_BUILD_TYPE RelWithDebInfo)

rosbuild_init()

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#uncomment if you have defined messages
#rosbuild_genmsg()
#uncomment if you have defined services
#rosbuild_gensrv()

#common commands for building c++ executables and libraries
#rosbuild_add_library(${PROJECT_NAME} src/example.cpp)
#target_link_libraries(${PROJECT_NAME} another_library)
#rosbuild_add_boost_directories()
#rosbuild_link_boost(${PROJECT_NAME} thread)
#rosbuild_add_executable(example examples/example.cpp)
#target_link_libraries(example ${PROJECT_NAME})
#rosbuild_add_executable(talker src/talker.cpp)
#rosbuild_add_executable(listener src/listener.cpp)

#rosbuild_add_executable(add_two_ints_server src/add_two_ints_server.cpp)
#rosbuild_add_executable(add_two_ints_client src/add_two_ints_client.cpp)

rosbuild_add_library(environment src/Environment.cpp)
rosbuild_add_compile_flags(environment -lm -std=c++0x -lpng)

rosbuild_add_library(robot src/Robot.cpp)
rosbuild_add_compile_flags(robot -lm -std=c++0x)
#rosbuild_add_compile_flags(lib_environment )



rosbuild_add_executable(robot_simulator src/robot_simulator.cpp)
rosbuild_add_compile_flags(robot_simulator -std=c++0x -lpthread)

rosbuild_add_executable(grid_processor src/grid_processor.cpp)
rosbuild_add_compile_flags(grid_processor -std=c++0x)
