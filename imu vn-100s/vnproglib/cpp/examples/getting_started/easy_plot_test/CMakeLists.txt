cmake_minimum_required(VERSION 3.0)
project(easy_plot_test)

# Set policy CMP0072 to suppress the OpenGL warning
if(POLICY CMP0072)
  cmake_policy(SET CMP0072 NEW)
endif()

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find GLUT
find_package(GLUT REQUIRED)

# Include directories
include_directories(${OPENGL_INCLUDE_DIR} ${GLUT_INCLUDE_DIR})

# Your source files
set(SOURCES main.cpp)

# Добавьте поддиректорию внешней библиотеки
add_subdirectory(easy_plot_cpp)

# Add executable
add_executable(easy_plot_test ${SOURCES})

# Подключите внешнюю библиотеку к вашему проекту
target_link_libraries(easy_plot_test PRIVATE ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
target_link_libraries(easy_plot_test PRIVATE easy_plot_cpp_lib)