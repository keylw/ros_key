cmake_minimum_required(VERSION 2.8.3)
project(collector)


find_package(catkin REQUIRED COMPONENTS
  roscpp
)

###################################
## catkin specific configuration ##
###################################
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES tms_ss_whs1
#  CATKIN_DEPENDS roscpp tms_msg_db tms_msg_ss
#  DEPENDS system_lib
)

###########
## Build ##
###########

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

