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

# Utility rule file for picotls-cli.

# Include the progress variables for this target.
include CMakeFiles/picotls-cli.dir/progress.make

CMakeFiles/picotls-cli: CMakeFiles/picotls-cli-complete


CMakeFiles/picotls-cli-complete: picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-install
CMakeFiles/picotls-cli-complete: picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-mkdir
CMakeFiles/picotls-cli-complete: picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-download
CMakeFiles/picotls-cli-complete: picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-update
CMakeFiles/picotls-cli-complete: picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-patch
CMakeFiles/picotls-cli-complete: picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-configure
CMakeFiles/picotls-cli-complete: picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-build
CMakeFiles/picotls-cli-complete: picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-install
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/magic/h2o-mphttp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'picotls-cli'"
	/usr/bin/cmake -E make_directory /home/magic/h2o-mphttp/CMakeFiles
	/usr/bin/cmake -E touch /home/magic/h2o-mphttp/CMakeFiles/picotls-cli-complete
	/usr/bin/cmake -E touch /home/magic/h2o-mphttp/picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-done

picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-install: picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-build
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/magic/h2o-mphttp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Performing install step for 'picotls-cli'"
	cd /home/magic/h2o-mphttp/picotls && true
	cd /home/magic/h2o-mphttp/picotls && /usr/bin/cmake -E touch /home/magic/h2o-mphttp/picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-install

picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/magic/h2o-mphttp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Creating directories for 'picotls-cli'"
	/usr/bin/cmake -E make_directory /home/magic/h2o-mphttp/deps/picotls
	/usr/bin/cmake -E make_directory /home/magic/h2o-mphttp/picotls
	/usr/bin/cmake -E make_directory /home/magic/h2o-mphttp/picotls-cli-prefix
	/usr/bin/cmake -E make_directory /home/magic/h2o-mphttp/picotls-cli-prefix/tmp
	/usr/bin/cmake -E make_directory /home/magic/h2o-mphttp/picotls-cli-prefix/src/picotls-cli-stamp
	/usr/bin/cmake -E make_directory /home/magic/h2o-mphttp/picotls-cli-prefix/src
	/usr/bin/cmake -E touch /home/magic/h2o-mphttp/picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-mkdir

picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-download: picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/magic/h2o-mphttp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "No download step for 'picotls-cli'"
	/usr/bin/cmake -E echo_append
	/usr/bin/cmake -E touch /home/magic/h2o-mphttp/picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-download

picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-update: picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/magic/h2o-mphttp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "No update step for 'picotls-cli'"
	/usr/bin/cmake -E echo_append
	/usr/bin/cmake -E touch /home/magic/h2o-mphttp/picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-update

picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-patch: picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/magic/h2o-mphttp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "No patch step for 'picotls-cli'"
	/usr/bin/cmake -E echo_append
	/usr/bin/cmake -E touch /home/magic/h2o-mphttp/picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-patch

picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-configure: picotls-cli-prefix/tmp/picotls-cli-cfgcmd.txt
picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-configure: picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-update
picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-configure: picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/magic/h2o-mphttp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Performing configure step for 'picotls-cli'"
	cd /home/magic/h2o-mphttp/picotls && /usr/bin/cmake -DOPENSSL_ROOT_DIR=/usr /home/magic/h2o-mphttp/deps/picotls
	cd /home/magic/h2o-mphttp/picotls && /usr/bin/cmake -E touch /home/magic/h2o-mphttp/picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-configure

picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-build: picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/magic/h2o-mphttp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Performing build step for 'picotls-cli'"
	cd /home/magic/h2o-mphttp/picotls && /usr/bin/make cli
	cd /home/magic/h2o-mphttp/picotls && /usr/bin/cmake -E touch /home/magic/h2o-mphttp/picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-build

picotls-cli: CMakeFiles/picotls-cli
picotls-cli: CMakeFiles/picotls-cli-complete
picotls-cli: picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-install
picotls-cli: picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-mkdir
picotls-cli: picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-download
picotls-cli: picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-update
picotls-cli: picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-patch
picotls-cli: picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-configure
picotls-cli: picotls-cli-prefix/src/picotls-cli-stamp/picotls-cli-build
picotls-cli: CMakeFiles/picotls-cli.dir/build.make

.PHONY : picotls-cli

# Rule to build all files generated by this target.
CMakeFiles/picotls-cli.dir/build: picotls-cli

.PHONY : CMakeFiles/picotls-cli.dir/build

CMakeFiles/picotls-cli.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/picotls-cli.dir/cmake_clean.cmake
.PHONY : CMakeFiles/picotls-cli.dir/clean

CMakeFiles/picotls-cli.dir/depend:
	cd /home/magic/h2o-mphttp && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/magic/h2o-mphttp /home/magic/h2o-mphttp /home/magic/h2o-mphttp /home/magic/h2o-mphttp /home/magic/h2o-mphttp/CMakeFiles/picotls-cli.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/picotls-cli.dir/depend

