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
include CMakeFiles/h2o-httpclient.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/h2o-httpclient.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/h2o-httpclient.dir/flags.make

CMakeFiles/h2o-httpclient.dir/src/httpclient.c.o: CMakeFiles/h2o-httpclient.dir/flags.make
CMakeFiles/h2o-httpclient.dir/src/httpclient.c.o: src/httpclient.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/magic/h2o-mphttp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/h2o-httpclient.dir/src/httpclient.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/h2o-httpclient.dir/src/httpclient.c.o   -c /home/magic/h2o-mphttp/src/httpclient.c

CMakeFiles/h2o-httpclient.dir/src/httpclient.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/h2o-httpclient.dir/src/httpclient.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/magic/h2o-mphttp/src/httpclient.c > CMakeFiles/h2o-httpclient.dir/src/httpclient.c.i

CMakeFiles/h2o-httpclient.dir/src/httpclient.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/h2o-httpclient.dir/src/httpclient.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/magic/h2o-mphttp/src/httpclient.c -o CMakeFiles/h2o-httpclient.dir/src/httpclient.c.s

CMakeFiles/h2o-httpclient.dir/src/httpclient.c.o.requires:

.PHONY : CMakeFiles/h2o-httpclient.dir/src/httpclient.c.o.requires

CMakeFiles/h2o-httpclient.dir/src/httpclient.c.o.provides: CMakeFiles/h2o-httpclient.dir/src/httpclient.c.o.requires
	$(MAKE) -f CMakeFiles/h2o-httpclient.dir/build.make CMakeFiles/h2o-httpclient.dir/src/httpclient.c.o.provides.build
.PHONY : CMakeFiles/h2o-httpclient.dir/src/httpclient.c.o.provides

CMakeFiles/h2o-httpclient.dir/src/httpclient.c.o.provides.build: CMakeFiles/h2o-httpclient.dir/src/httpclient.c.o


# Object files for target h2o-httpclient
h2o__httpclient_OBJECTS = \
"CMakeFiles/h2o-httpclient.dir/src/httpclient.c.o"

# External object files for target h2o-httpclient
h2o__httpclient_EXTERNAL_OBJECTS =

bin/h2o-httpclient: CMakeFiles/h2o-httpclient.dir/src/httpclient.c.o
bin/h2o-httpclient: CMakeFiles/h2o-httpclient.dir/build.make
bin/h2o-httpclient: libh2o-evloop.a
bin/h2o-httpclient: /usr/lib/i386-linux-gnu/libz.so
bin/h2o-httpclient: /usr/lib/i386-linux-gnu/libssl.so
bin/h2o-httpclient: /usr/lib/i386-linux-gnu/libcrypto.so
bin/h2o-httpclient: CMakeFiles/h2o-httpclient.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/magic/h2o-mphttp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable bin/h2o-httpclient"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/h2o-httpclient.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/h2o-httpclient.dir/build: bin/h2o-httpclient

.PHONY : CMakeFiles/h2o-httpclient.dir/build

CMakeFiles/h2o-httpclient.dir/requires: CMakeFiles/h2o-httpclient.dir/src/httpclient.c.o.requires

.PHONY : CMakeFiles/h2o-httpclient.dir/requires

CMakeFiles/h2o-httpclient.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/h2o-httpclient.dir/cmake_clean.cmake
.PHONY : CMakeFiles/h2o-httpclient.dir/clean

CMakeFiles/h2o-httpclient.dir/depend:
	cd /home/magic/h2o-mphttp && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/magic/h2o-mphttp /home/magic/h2o-mphttp /home/magic/h2o-mphttp /home/magic/h2o-mphttp /home/magic/h2o-mphttp/CMakeFiles/h2o-httpclient.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/h2o-httpclient.dir/depend

