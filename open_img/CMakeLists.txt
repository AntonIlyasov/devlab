cmake_minimum_required(VERSION 3.2)
project(open_img)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED )
add_executable(open_img main.cpp)
target_link_libraries( open_img ${OpenCV_LIBS} )