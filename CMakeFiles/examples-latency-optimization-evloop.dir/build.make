# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/magic/h2o-mphttp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/magic/h2o-mphttp

# Include any dependencies generated for this target.
include CMakeFiles/examples-latency-optimization-evloop.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/examples-latency-optimization-evloop.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/examples-latency-optimization-evloop.dir/flags.make

CMakeFiles/examples-latency-optimization-evloop.dir/examples/libh2o/latency-optimization.c.o: CMakeFiles/examples-latency-optimization-evloop.dir/flags.make
CMakeFiles/examples-latency-optimization-evloop.dir/examples/libh2o/latency-optimization.c.o: examples/libh2o/latency-optimization.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/magic/h2o-mphttp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/examples-latency-optimization-evloop.dir/examples/libh2o/latency-optimization.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/examples-latency-optimization-evloop.dir/examples/libh2o/latency-optimization.c.o   -c /home/magic/h2o-mphttp/examples/libh2o/latency-optimization.c

CMakeFiles/examples-latency-optimization-evloop.dir/examples/libh2o/latency-optimization.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/examples-latency-optimization-evloop.dir/examples/libh2o/latency-optimization.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/magic/h2o-mphttp/examples/libh2o/latency-optimization.c > CMakeFiles/examples-latency-optimization-evloop.dir/examples/libh2o/latency-optimization.c.i

CMakeFiles/examples-latency-optimization-evloop.dir/examples/libh2o/latency-optimization.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/examples-latency-optimization-evloop.dir/examples/libh2o/latency-optimization.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/magic/h2o-mphttp/examples/libh2o/latency-optimization.c -o CMakeFiles/examples-latency-optimization-evloop.dir/examples/libh2o/latency-optimization.c.s

CMakeFiles/examples-latency-optimization-evloop.dir/examples/libh2o/latency-optimization.c.o.requires:

.PHONY : CMakeFiles/examples-latency-optimization-evloop.dir/examples/libh2o/latency-optimization.c.o.requires

CMakeFiles/examples-latency-optimization-evloop.dir/examples/libh2o/latency-optimization.c.o.provides: CMakeFiles/examples-latency-optimization-evloop.dir/examples/libh2o/latency-optimization.c.o.requires
	$(MAKE) -f CMakeFiles/examples-latency-optimization-evloop.dir/build.make CMakeFiles/examples-latency-optimization-evloop.dir/examples/libh2o/latency-optimization.c.o.provides.build
.PHONY : CMakeFiles/examples-latency-optimization-evloop.dir/examples/libh2o/latency-optimization.c.o.provides

CMakeFiles/examples-latency-optimization-evloop.dir/examples/libh2o/latency-optimization.c.o.provides.build: CMakeFiles/examples-latency-optimization-evloop.dir/examples/libh2o/latency-optimization.c.o


# Object files for target examples-latency-optimization-evloop
examples__latency__optimization__evloop_OBJECTS = \
"CMakeFiles/examples-latency-optimization-evloop.dir/examples/libh2o/latency-optimization.c.o"

# External object files for target examples-latency-optimization-evloop
examples__latency__optimization__evloop_EXTERNAL_OBJECTS =

examples-latency-optimization-evloop: CMakeFiles/examples-latency-optimization-evloop.dir/examples/libh2o/latency-optimization.c.o
examples-latency-optimization-evloop: CMakeFiles/examples-latency-optimization-evloop.dir/build.make
examples-latency-optimization-evloop: libh2o-evloop.a
examples-latency-optimization-evloop: /usr/lib/i386-linux-gnu/libz.so
examples-latency-optimization-evloop: /usr/lib/i386-linux-gnu/libssl.so
examples-latency-optimization-evloop: /usr/lib/i386-linux-gnu/libcrypto.so
examples-latency-optimization-evloop: CMakeFiles/examples-latency-optimization-evloop.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/magic/h2o-mphttp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable examples-latency-optimization-evloop"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/examples-latency-optimization-evloop.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/examples-latency-optimization-evloop.dir/build: examples-latency-optimization-evloop

.PHONY : CMakeFiles/examples-latency-optimization-evloop.dir/build

CMakeFiles/examples-latency-optimization-evloop.dir/requires: CMakeFiles/examples-latency-optimization-evloop.dir/examples/libh2o/latency-optimization.c.o.requires

.PHONY : CMakeFiles/examples-latency-optimization-evloop.dir/requires

CMakeFiles/examples-latency-optimization-evloop.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/examples-latency-optimization-evloop.dir/cmake_clean.cmake
.PHONY : CMakeFiles/examples-latency-optimization-evloop.dir/clean

CMakeFiles/examples-latency-optimization-evloop.dir/depend:
	cd /home/magic/h2o-mphttp && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/magic/h2o-mphttp /home/magic/h2o-mphttp /home/magic/h2o-mphttp /home/magic/h2o-mphttp /home/magic/h2o-mphttp/CMakeFiles/examples-latency-optimization-evloop.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/examples-latency-optimization-evloop.dir/depend

