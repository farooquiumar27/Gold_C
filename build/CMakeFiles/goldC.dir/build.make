# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/umar/steps/1/dev/hydrogen

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/umar/steps/1/dev/hydrogen/build

# Include any dependencies generated for this target.
include CMakeFiles/goldC.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/goldC.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/goldC.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/goldC.dir/flags.make

CMakeFiles/goldC.dir/src/main.cpp.o: CMakeFiles/goldC.dir/flags.make
CMakeFiles/goldC.dir/src/main.cpp.o: /home/umar/steps/1/dev/hydrogen/src/main.cpp
CMakeFiles/goldC.dir/src/main.cpp.o: CMakeFiles/goldC.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/umar/steps/1/dev/hydrogen/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/goldC.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/goldC.dir/src/main.cpp.o -MF CMakeFiles/goldC.dir/src/main.cpp.o.d -o CMakeFiles/goldC.dir/src/main.cpp.o -c /home/umar/steps/1/dev/hydrogen/src/main.cpp

CMakeFiles/goldC.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/goldC.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/umar/steps/1/dev/hydrogen/src/main.cpp > CMakeFiles/goldC.dir/src/main.cpp.i

CMakeFiles/goldC.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/goldC.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/umar/steps/1/dev/hydrogen/src/main.cpp -o CMakeFiles/goldC.dir/src/main.cpp.s

# Object files for target goldC
goldC_OBJECTS = \
"CMakeFiles/goldC.dir/src/main.cpp.o"

# External object files for target goldC
goldC_EXTERNAL_OBJECTS =

goldC: CMakeFiles/goldC.dir/src/main.cpp.o
goldC: CMakeFiles/goldC.dir/build.make
goldC: CMakeFiles/goldC.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/umar/steps/1/dev/hydrogen/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable goldC"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/goldC.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/goldC.dir/build: goldC
.PHONY : CMakeFiles/goldC.dir/build

CMakeFiles/goldC.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/goldC.dir/cmake_clean.cmake
.PHONY : CMakeFiles/goldC.dir/clean

CMakeFiles/goldC.dir/depend:
	cd /home/umar/steps/1/dev/hydrogen/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/umar/steps/1/dev/hydrogen /home/umar/steps/1/dev/hydrogen /home/umar/steps/1/dev/hydrogen/build /home/umar/steps/1/dev/hydrogen/build /home/umar/steps/1/dev/hydrogen/build/CMakeFiles/goldC.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/goldC.dir/depend

