# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/anton20241/devlab/SteelRain for DISworking altitude/my_client"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/anton20241/devlab/SteelRain for DISworking altitude/my_client/build"

# Include any dependencies generated for this target.
include CMakeFiles/client.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/client.dir/flags.make

CMakeFiles/client.dir/client.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/client.cpp.o: ../client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/anton20241/devlab/SteelRain for DISworking altitude/my_client/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/client.dir/client.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client.dir/client.cpp.o -c "/home/anton20241/devlab/SteelRain for DISworking altitude/my_client/client.cpp"

CMakeFiles/client.dir/client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/client.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/anton20241/devlab/SteelRain for DISworking altitude/my_client/client.cpp" > CMakeFiles/client.dir/client.cpp.i

CMakeFiles/client.dir/client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/client.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/anton20241/devlab/SteelRain for DISworking altitude/my_client/client.cpp" -o CMakeFiles/client.dir/client.cpp.s

# Object files for target client
client_OBJECTS = \
"CMakeFiles/client.dir/client.cpp.o"

# External object files for target client
client_EXTERNAL_OBJECTS =

client: CMakeFiles/client.dir/client.cpp.o
client: CMakeFiles/client.dir/build.make
client: /usr/lib/x86_64-linux-gnu/libboost_system.so.1.71.0
client: CMakeFiles/client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/anton20241/devlab/SteelRain for DISworking altitude/my_client/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/client.dir/build: client

.PHONY : CMakeFiles/client.dir/build

CMakeFiles/client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/client.dir/clean

CMakeFiles/client.dir/depend:
	cd "/home/anton20241/devlab/SteelRain for DISworking altitude/my_client/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/anton20241/devlab/SteelRain for DISworking altitude/my_client" "/home/anton20241/devlab/SteelRain for DISworking altitude/my_client" "/home/anton20241/devlab/SteelRain for DISworking altitude/my_client/build" "/home/anton20241/devlab/SteelRain for DISworking altitude/my_client/build" "/home/anton20241/devlab/SteelRain for DISworking altitude/my_client/build/CMakeFiles/client.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/client.dir/depend

