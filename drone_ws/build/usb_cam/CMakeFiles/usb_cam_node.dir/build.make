# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/anton202/.local/lib/python3.10/site-packages/cmake/data/bin/cmake

# The command to remove a file.
RM = /home/anton202/.local/lib/python3.10/site-packages/cmake/data/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/anton202/devlab/drone_ws/src/usb_cam

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/anton202/devlab/drone_ws/build/usb_cam

# Include any dependencies generated for this target.
include CMakeFiles/usb_cam_node.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/usb_cam_node.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/usb_cam_node.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/usb_cam_node.dir/flags.make

CMakeFiles/usb_cam_node.dir/src/ros2/usb_cam_node.cpp.o: CMakeFiles/usb_cam_node.dir/flags.make
CMakeFiles/usb_cam_node.dir/src/ros2/usb_cam_node.cpp.o: /home/anton202/devlab/drone_ws/src/usb_cam/src/ros2/usb_cam_node.cpp
CMakeFiles/usb_cam_node.dir/src/ros2/usb_cam_node.cpp.o: CMakeFiles/usb_cam_node.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/anton202/devlab/drone_ws/build/usb_cam/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/usb_cam_node.dir/src/ros2/usb_cam_node.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/usb_cam_node.dir/src/ros2/usb_cam_node.cpp.o -MF CMakeFiles/usb_cam_node.dir/src/ros2/usb_cam_node.cpp.o.d -o CMakeFiles/usb_cam_node.dir/src/ros2/usb_cam_node.cpp.o -c /home/anton202/devlab/drone_ws/src/usb_cam/src/ros2/usb_cam_node.cpp

CMakeFiles/usb_cam_node.dir/src/ros2/usb_cam_node.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/usb_cam_node.dir/src/ros2/usb_cam_node.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/anton202/devlab/drone_ws/src/usb_cam/src/ros2/usb_cam_node.cpp > CMakeFiles/usb_cam_node.dir/src/ros2/usb_cam_node.cpp.i

CMakeFiles/usb_cam_node.dir/src/ros2/usb_cam_node.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/usb_cam_node.dir/src/ros2/usb_cam_node.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/anton202/devlab/drone_ws/src/usb_cam/src/ros2/usb_cam_node.cpp -o CMakeFiles/usb_cam_node.dir/src/ros2/usb_cam_node.cpp.s

# Object files for target usb_cam_node
usb_cam_node_OBJECTS = \
"CMakeFiles/usb_cam_node.dir/src/ros2/usb_cam_node.cpp.o"

# External object files for target usb_cam_node
usb_cam_node_EXTERNAL_OBJECTS =

libusb_cam_node.so: CMakeFiles/usb_cam_node.dir/src/ros2/usb_cam_node.cpp.o
libusb_cam_node.so: CMakeFiles/usb_cam_node.dir/build.make
libusb_cam_node.so: /opt/ros/iron/lib/libcomponent_manager.so
libusb_cam_node.so: /opt/ros/iron/lib/libcv_bridge.so
libusb_cam_node.so: /opt/ros/iron/lib/libstd_srvs__rosidl_typesupport_fastrtps_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libstd_srvs__rosidl_typesupport_introspection_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libstd_srvs__rosidl_typesupport_fastrtps_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/libstd_srvs__rosidl_typesupport_introspection_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/libstd_srvs__rosidl_typesupport_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/libstd_srvs__rosidl_generator_py.so
libusb_cam_node.so: /opt/ros/iron/lib/libcamera_info_manager.so
libusb_cam_node.so: /opt/ros/iron/lib/x86_64-linux-gnu/libimage_transport.so
libusb_cam_node.so: libusb_cam.so
libusb_cam_node.so: /opt/ros/iron/lib/libclass_loader.so
libusb_cam_node.so: /usr/lib/x86_64-linux-gnu/libconsole_bridge.so.1.0
libusb_cam_node.so: /opt/ros/iron/lib/libcomposition_interfaces__rosidl_typesupport_fastrtps_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libcomposition_interfaces__rosidl_typesupport_introspection_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libcomposition_interfaces__rosidl_typesupport_fastrtps_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/libcomposition_interfaces__rosidl_typesupport_introspection_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/libcomposition_interfaces__rosidl_typesupport_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/libcomposition_interfaces__rosidl_generator_py.so
libusb_cam_node.so: /opt/ros/iron/lib/libcomposition_interfaces__rosidl_typesupport_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libcomposition_interfaces__rosidl_generator_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libstd_srvs__rosidl_typesupport_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libstd_srvs__rosidl_generator_c.so
libusb_cam_node.so: /opt/ros/iron/lib/librclcpp_lifecycle.so
libusb_cam_node.so: /opt/ros/iron/lib/librcl_lifecycle.so
libusb_cam_node.so: /opt/ros/iron/lib/liblifecycle_msgs__rosidl_typesupport_fastrtps_c.so
libusb_cam_node.so: /opt/ros/iron/lib/liblifecycle_msgs__rosidl_typesupport_introspection_c.so
libusb_cam_node.so: /opt/ros/iron/lib/liblifecycle_msgs__rosidl_typesupport_fastrtps_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/liblifecycle_msgs__rosidl_typesupport_introspection_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/liblifecycle_msgs__rosidl_typesupport_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/liblifecycle_msgs__rosidl_generator_py.so
libusb_cam_node.so: /opt/ros/iron/lib/liblifecycle_msgs__rosidl_typesupport_c.so
libusb_cam_node.so: /opt/ros/iron/lib/liblifecycle_msgs__rosidl_generator_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libsensor_msgs__rosidl_typesupport_fastrtps_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libgeometry_msgs__rosidl_typesupport_fastrtps_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libstd_msgs__rosidl_typesupport_fastrtps_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libsensor_msgs__rosidl_typesupport_fastrtps_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/libgeometry_msgs__rosidl_typesupport_fastrtps_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/libstd_msgs__rosidl_typesupport_fastrtps_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/libsensor_msgs__rosidl_typesupport_introspection_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libgeometry_msgs__rosidl_typesupport_introspection_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libstd_msgs__rosidl_typesupport_introspection_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libsensor_msgs__rosidl_typesupport_introspection_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/libgeometry_msgs__rosidl_typesupport_introspection_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/libstd_msgs__rosidl_typesupport_introspection_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/libsensor_msgs__rosidl_generator_py.so
libusb_cam_node.so: /opt/ros/iron/lib/libsensor_msgs__rosidl_typesupport_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libgeometry_msgs__rosidl_generator_py.so
libusb_cam_node.so: /opt/ros/iron/lib/libstd_msgs__rosidl_generator_py.so
libusb_cam_node.so: /opt/ros/iron/lib/libgeometry_msgs__rosidl_typesupport_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libstd_msgs__rosidl_typesupport_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libsensor_msgs__rosidl_typesupport_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/libsensor_msgs__rosidl_generator_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libgeometry_msgs__rosidl_typesupport_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/libstd_msgs__rosidl_typesupport_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/libgeometry_msgs__rosidl_generator_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libstd_msgs__rosidl_generator_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libmessage_filters.so
libusb_cam_node.so: /opt/ros/iron/lib/librclcpp.so
libusb_cam_node.so: /opt/ros/iron/lib/liblibstatistics_collector.so
libusb_cam_node.so: /opt/ros/iron/lib/librcl.so
libusb_cam_node.so: /opt/ros/iron/lib/librcl_logging_interface.so
libusb_cam_node.so: /opt/ros/iron/lib/librmw_implementation.so
libusb_cam_node.so: /opt/ros/iron/lib/libament_index_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/libtype_description_interfaces__rosidl_typesupport_fastrtps_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libtype_description_interfaces__rosidl_typesupport_introspection_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libtype_description_interfaces__rosidl_typesupport_fastrtps_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/libtype_description_interfaces__rosidl_typesupport_introspection_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/libtype_description_interfaces__rosidl_typesupport_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/libtype_description_interfaces__rosidl_generator_py.so
libusb_cam_node.so: /opt/ros/iron/lib/libtype_description_interfaces__rosidl_typesupport_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libtype_description_interfaces__rosidl_generator_c.so
libusb_cam_node.so: /opt/ros/iron/lib/librcl_interfaces__rosidl_typesupport_fastrtps_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libservice_msgs__rosidl_typesupport_fastrtps_c.so
libusb_cam_node.so: /opt/ros/iron/lib/librcl_interfaces__rosidl_typesupport_introspection_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libservice_msgs__rosidl_typesupport_introspection_c.so
libusb_cam_node.so: /opt/ros/iron/lib/librcl_interfaces__rosidl_typesupport_fastrtps_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/libservice_msgs__rosidl_typesupport_fastrtps_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/librcl_interfaces__rosidl_typesupport_introspection_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/libservice_msgs__rosidl_typesupport_introspection_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/librcl_interfaces__rosidl_typesupport_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/libservice_msgs__rosidl_typesupport_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/librcl_interfaces__rosidl_generator_py.so
libusb_cam_node.so: /opt/ros/iron/lib/librcl_interfaces__rosidl_typesupport_c.so
libusb_cam_node.so: /opt/ros/iron/lib/librcl_interfaces__rosidl_generator_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libservice_msgs__rosidl_generator_py.so
libusb_cam_node.so: /opt/ros/iron/lib/libservice_msgs__rosidl_typesupport_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libservice_msgs__rosidl_generator_c.so
libusb_cam_node.so: /opt/ros/iron/lib/librcl_yaml_param_parser.so
libusb_cam_node.so: /opt/ros/iron/lib/librosgraph_msgs__rosidl_typesupport_fastrtps_c.so
libusb_cam_node.so: /opt/ros/iron/lib/librosgraph_msgs__rosidl_typesupport_fastrtps_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/librosgraph_msgs__rosidl_typesupport_introspection_c.so
libusb_cam_node.so: /opt/ros/iron/lib/librosgraph_msgs__rosidl_typesupport_introspection_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/librosgraph_msgs__rosidl_typesupport_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/librosgraph_msgs__rosidl_generator_py.so
libusb_cam_node.so: /opt/ros/iron/lib/librosgraph_msgs__rosidl_typesupport_c.so
libusb_cam_node.so: /opt/ros/iron/lib/librosgraph_msgs__rosidl_generator_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libstatistics_msgs__rosidl_typesupport_fastrtps_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libbuiltin_interfaces__rosidl_typesupport_fastrtps_c.so
libusb_cam_node.so: /opt/ros/iron/lib/librosidl_typesupport_fastrtps_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libstatistics_msgs__rosidl_typesupport_fastrtps_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/libbuiltin_interfaces__rosidl_typesupport_fastrtps_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/librosidl_typesupport_fastrtps_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/libfastcdr.so.1.0.27
libusb_cam_node.so: /opt/ros/iron/lib/librmw.so
libusb_cam_node.so: /opt/ros/iron/lib/librosidl_dynamic_typesupport.so
libusb_cam_node.so: /opt/ros/iron/lib/libstatistics_msgs__rosidl_typesupport_introspection_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libbuiltin_interfaces__rosidl_typesupport_introspection_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libstatistics_msgs__rosidl_typesupport_introspection_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/libbuiltin_interfaces__rosidl_typesupport_introspection_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/librosidl_typesupport_introspection_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/librosidl_typesupport_introspection_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libstatistics_msgs__rosidl_typesupport_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/libbuiltin_interfaces__rosidl_typesupport_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/librosidl_typesupport_cpp.so
libusb_cam_node.so: /opt/ros/iron/lib/libstatistics_msgs__rosidl_generator_py.so
libusb_cam_node.so: /opt/ros/iron/lib/libbuiltin_interfaces__rosidl_generator_py.so
libusb_cam_node.so: /opt/ros/iron/lib/libstatistics_msgs__rosidl_typesupport_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libbuiltin_interfaces__rosidl_typesupport_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libstatistics_msgs__rosidl_generator_c.so
libusb_cam_node.so: /opt/ros/iron/lib/libbuiltin_interfaces__rosidl_generator_c.so
libusb_cam_node.so: /opt/ros/iron/lib/librosidl_typesupport_c.so
libusb_cam_node.so: /opt/ros/iron/lib/librcpputils.so
libusb_cam_node.so: /opt/ros/iron/lib/librosidl_runtime_c.so
libusb_cam_node.so: /usr/lib/x86_64-linux-gnu/libpython3.10.so
libusb_cam_node.so: /opt/ros/iron/lib/libtracetools.so
libusb_cam_node.so: /opt/ros/iron/lib/librcutils.so
libusb_cam_node.so: /usr/local/lib/libopencv_gapi.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_stitching.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_alphamat.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_aruco.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_barcode.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_bgsegm.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_bioinspired.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_ccalib.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_cvv.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_dnn_objdetect.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_dnn_superres.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_dpm.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_face.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_freetype.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_fuzzy.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_hdf.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_hfs.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_img_hash.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_intensity_transform.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_line_descriptor.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_mcc.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_quality.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_rapid.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_reg.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_rgbd.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_saliency.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_stereo.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_structured_light.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_phase_unwrapping.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_superres.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_optflow.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_surface_matching.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_tracking.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_highgui.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_datasets.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_plot.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_text.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_videostab.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_videoio.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_viz.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_wechat_qrcode.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_xfeatures2d.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_ml.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_shape.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_ximgproc.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_video.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_xobjdetect.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_imgcodecs.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_objdetect.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_calib3d.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_dnn.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_features2d.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_flann.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_xphoto.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_photo.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_imgproc.so.4.7.0
libusb_cam_node.so: /usr/local/lib/libopencv_core.so.4.7.0
libusb_cam_node.so: CMakeFiles/usb_cam_node.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/anton202/devlab/drone_ws/build/usb_cam/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libusb_cam_node.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/usb_cam_node.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/usb_cam_node.dir/build: libusb_cam_node.so
.PHONY : CMakeFiles/usb_cam_node.dir/build

CMakeFiles/usb_cam_node.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/usb_cam_node.dir/cmake_clean.cmake
.PHONY : CMakeFiles/usb_cam_node.dir/clean

CMakeFiles/usb_cam_node.dir/depend:
	cd /home/anton202/devlab/drone_ws/build/usb_cam && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/anton202/devlab/drone_ws/src/usb_cam /home/anton202/devlab/drone_ws/src/usb_cam /home/anton202/devlab/drone_ws/build/usb_cam /home/anton202/devlab/drone_ws/build/usb_cam /home/anton202/devlab/drone_ws/build/usb_cam/CMakeFiles/usb_cam_node.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/usb_cam_node.dir/depend

