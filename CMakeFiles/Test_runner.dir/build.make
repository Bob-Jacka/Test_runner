# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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
CMAKE_SOURCE_DIR = /home/kirill/Downloads/Test_runner

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kirill/Downloads/Test_runner

# Include any dependencies generated for this target.
include CMakeFiles/Test_runner.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Test_runner.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Test_runner.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Test_runner.dir/flags.make

CMakeFiles/Test_runner.dir/codegen:
.PHONY : CMakeFiles/Test_runner.dir/codegen

CMakeFiles/Test_runner.dir/main.cpp.o: CMakeFiles/Test_runner.dir/flags.make
CMakeFiles/Test_runner.dir/main.cpp.o: main.cpp
CMakeFiles/Test_runner.dir/main.cpp.o: CMakeFiles/Test_runner.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kirill/Downloads/Test_runner/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Test_runner.dir/main.cpp.o"
	/usr/bin/gcc $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Test_runner.dir/main.cpp.o -MF CMakeFiles/Test_runner.dir/main.cpp.o.d -o CMakeFiles/Test_runner.dir/main.cpp.o -c /home/kirill/Downloads/Test_runner/main.cpp

CMakeFiles/Test_runner.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Test_runner.dir/main.cpp.i"
	/usr/bin/gcc $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kirill/Downloads/Test_runner/main.cpp > CMakeFiles/Test_runner.dir/main.cpp.i

CMakeFiles/Test_runner.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Test_runner.dir/main.cpp.s"
	/usr/bin/gcc $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kirill/Downloads/Test_runner/main.cpp -o CMakeFiles/Test_runner.dir/main.cpp.s

# Object files for target Test_runner
Test_runner_OBJECTS = \
"CMakeFiles/Test_runner.dir/main.cpp.o"

# External object files for target Test_runner
Test_runner_EXTERNAL_OBJECTS =

Test_runner: CMakeFiles/Test_runner.dir/main.cpp.o
Test_runner: CMakeFiles/Test_runner.dir/build.make
Test_runner: CMakeFiles/Test_runner.dir/compiler_depend.ts
Test_runner: CMakeFiles/Test_runner.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/kirill/Downloads/Test_runner/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Test_runner"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Test_runner.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Test_runner.dir/build: Test_runner
.PHONY : CMakeFiles/Test_runner.dir/build

CMakeFiles/Test_runner.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Test_runner.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Test_runner.dir/clean

CMakeFiles/Test_runner.dir/depend:
	cd /home/kirill/Downloads/Test_runner && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kirill/Downloads/Test_runner /home/kirill/Downloads/Test_runner /home/kirill/Downloads/Test_runner /home/kirill/Downloads/Test_runner /home/kirill/Downloads/Test_runner/CMakeFiles/Test_runner.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/Test_runner.dir/depend
