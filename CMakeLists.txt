cmake_minimum_required( VERSION 2.8 )
project( BlurVideo )

find_package( OpenCV REQUIRED )
find_package( Boost REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories( ${Boost_INCLUDE_DIRS} )

add_executable( BlurVideo BlurVideo.cpp )
set_property( TARGET BlurVideo PROPERTY CXX_STANDARD 17 )
target_link_libraries( BlurVideo ${OpenCV_LIBS} )
