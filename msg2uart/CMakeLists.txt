cmake_minimum_required(VERSION 2.8.3)
project(ros_uart_communication)

## Compile as C++11, supported in ROS Kinetic and newer
#add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  geometry_msgs
  message_generation
)

add_message_files(
  FILES
  cmd.msg
  feedback.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
  #INCLUDE_DIRS include
  #LIBRARIES ros_uart_communication
  CATKIN_DEPENDS roscpp rospy std_msgs message_runtime
  # DEPENDS system_lib
)


## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(ros_uart_communication src/main.cpp
                                      src/SerialPort.cpp
                                      src/SerialNode.cpp)
target_link_libraries(ros_uart_communication ${catkin_LIBRARIES})


