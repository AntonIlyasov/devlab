cmake_minimum_required(VERSION 2.8)
PROJECT (my_client)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.71.0 REQUIRED COMPONENTS system)

if(Boost_FOUND)
include_directories(${Boost_INCLUDE_DIRS})

add_executable( client client.cpp )
target_link_libraries( client ${Boost_LIBRARIES} )

add_executable( client_recieve client_recieve.cpp )
target_link_libraries( client_recieve ${Boost_LIBRARIES} )

endif()

set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)
