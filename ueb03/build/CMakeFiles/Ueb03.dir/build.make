# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_SOURCE_DIR = /home/marcello/Uni/cg1_12/ueb03

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/marcello/Uni/cg1_12/ueb03/build

# Include any dependencies generated for this target.
include CMakeFiles/Ueb03.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Ueb03.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Ueb03.dir/flags.make

CMakeFiles/Ueb03.dir/src/debugGL.c.o: CMakeFiles/Ueb03.dir/flags.make
CMakeFiles/Ueb03.dir/src/debugGL.c.o: ../src/debugGL.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marcello/Uni/cg1_12/ueb03/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Ueb03.dir/src/debugGL.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Ueb03.dir/src/debugGL.c.o   -c /home/marcello/Uni/cg1_12/ueb03/src/debugGL.c

CMakeFiles/Ueb03.dir/src/debugGL.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Ueb03.dir/src/debugGL.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/marcello/Uni/cg1_12/ueb03/src/debugGL.c > CMakeFiles/Ueb03.dir/src/debugGL.c.i

CMakeFiles/Ueb03.dir/src/debugGL.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Ueb03.dir/src/debugGL.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/marcello/Uni/cg1_12/ueb03/src/debugGL.c -o CMakeFiles/Ueb03.dir/src/debugGL.c.s

CMakeFiles/Ueb03.dir/src/io.c.o: CMakeFiles/Ueb03.dir/flags.make
CMakeFiles/Ueb03.dir/src/io.c.o: ../src/io.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marcello/Uni/cg1_12/ueb03/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/Ueb03.dir/src/io.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Ueb03.dir/src/io.c.o   -c /home/marcello/Uni/cg1_12/ueb03/src/io.c

CMakeFiles/Ueb03.dir/src/io.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Ueb03.dir/src/io.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/marcello/Uni/cg1_12/ueb03/src/io.c > CMakeFiles/Ueb03.dir/src/io.c.i

CMakeFiles/Ueb03.dir/src/io.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Ueb03.dir/src/io.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/marcello/Uni/cg1_12/ueb03/src/io.c -o CMakeFiles/Ueb03.dir/src/io.c.s

CMakeFiles/Ueb03.dir/src/logic.c.o: CMakeFiles/Ueb03.dir/flags.make
CMakeFiles/Ueb03.dir/src/logic.c.o: ../src/logic.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marcello/Uni/cg1_12/ueb03/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/Ueb03.dir/src/logic.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Ueb03.dir/src/logic.c.o   -c /home/marcello/Uni/cg1_12/ueb03/src/logic.c

CMakeFiles/Ueb03.dir/src/logic.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Ueb03.dir/src/logic.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/marcello/Uni/cg1_12/ueb03/src/logic.c > CMakeFiles/Ueb03.dir/src/logic.c.i

CMakeFiles/Ueb03.dir/src/logic.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Ueb03.dir/src/logic.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/marcello/Uni/cg1_12/ueb03/src/logic.c -o CMakeFiles/Ueb03.dir/src/logic.c.s

CMakeFiles/Ueb03.dir/src/main.c.o: CMakeFiles/Ueb03.dir/flags.make
CMakeFiles/Ueb03.dir/src/main.c.o: ../src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marcello/Uni/cg1_12/ueb03/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/Ueb03.dir/src/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Ueb03.dir/src/main.c.o   -c /home/marcello/Uni/cg1_12/ueb03/src/main.c

CMakeFiles/Ueb03.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Ueb03.dir/src/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/marcello/Uni/cg1_12/ueb03/src/main.c > CMakeFiles/Ueb03.dir/src/main.c.i

CMakeFiles/Ueb03.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Ueb03.dir/src/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/marcello/Uni/cg1_12/ueb03/src/main.c -o CMakeFiles/Ueb03.dir/src/main.c.s

CMakeFiles/Ueb03.dir/src/scene.c.o: CMakeFiles/Ueb03.dir/flags.make
CMakeFiles/Ueb03.dir/src/scene.c.o: ../src/scene.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marcello/Uni/cg1_12/ueb03/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/Ueb03.dir/src/scene.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Ueb03.dir/src/scene.c.o   -c /home/marcello/Uni/cg1_12/ueb03/src/scene.c

CMakeFiles/Ueb03.dir/src/scene.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Ueb03.dir/src/scene.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/marcello/Uni/cg1_12/ueb03/src/scene.c > CMakeFiles/Ueb03.dir/src/scene.c.i

CMakeFiles/Ueb03.dir/src/scene.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Ueb03.dir/src/scene.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/marcello/Uni/cg1_12/ueb03/src/scene.c -o CMakeFiles/Ueb03.dir/src/scene.c.s

CMakeFiles/Ueb03.dir/src/stringOutput.c.o: CMakeFiles/Ueb03.dir/flags.make
CMakeFiles/Ueb03.dir/src/stringOutput.c.o: ../src/stringOutput.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marcello/Uni/cg1_12/ueb03/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/Ueb03.dir/src/stringOutput.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Ueb03.dir/src/stringOutput.c.o   -c /home/marcello/Uni/cg1_12/ueb03/src/stringOutput.c

CMakeFiles/Ueb03.dir/src/stringOutput.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Ueb03.dir/src/stringOutput.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/marcello/Uni/cg1_12/ueb03/src/stringOutput.c > CMakeFiles/Ueb03.dir/src/stringOutput.c.i

CMakeFiles/Ueb03.dir/src/stringOutput.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Ueb03.dir/src/stringOutput.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/marcello/Uni/cg1_12/ueb03/src/stringOutput.c -o CMakeFiles/Ueb03.dir/src/stringOutput.c.s

# Object files for target Ueb03
Ueb03_OBJECTS = \
"CMakeFiles/Ueb03.dir/src/debugGL.c.o" \
"CMakeFiles/Ueb03.dir/src/io.c.o" \
"CMakeFiles/Ueb03.dir/src/logic.c.o" \
"CMakeFiles/Ueb03.dir/src/main.c.o" \
"CMakeFiles/Ueb03.dir/src/scene.c.o" \
"CMakeFiles/Ueb03.dir/src/stringOutput.c.o"

# External object files for target Ueb03
Ueb03_EXTERNAL_OBJECTS =

Ueb03: CMakeFiles/Ueb03.dir/src/debugGL.c.o
Ueb03: CMakeFiles/Ueb03.dir/src/io.c.o
Ueb03: CMakeFiles/Ueb03.dir/src/logic.c.o
Ueb03: CMakeFiles/Ueb03.dir/src/main.c.o
Ueb03: CMakeFiles/Ueb03.dir/src/scene.c.o
Ueb03: CMakeFiles/Ueb03.dir/src/stringOutput.c.o
Ueb03: CMakeFiles/Ueb03.dir/build.make
Ueb03: /usr/lib/x86_64-linux-gnu/libGL.so
Ueb03: CMakeFiles/Ueb03.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/marcello/Uni/cg1_12/ueb03/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking C executable Ueb03"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Ueb03.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Ueb03.dir/build: Ueb03

.PHONY : CMakeFiles/Ueb03.dir/build

CMakeFiles/Ueb03.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Ueb03.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Ueb03.dir/clean

CMakeFiles/Ueb03.dir/depend:
	cd /home/marcello/Uni/cg1_12/ueb03/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/marcello/Uni/cg1_12/ueb03 /home/marcello/Uni/cg1_12/ueb03 /home/marcello/Uni/cg1_12/ueb03/build /home/marcello/Uni/cg1_12/ueb03/build /home/marcello/Uni/cg1_12/ueb03/build/CMakeFiles/Ueb03.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Ueb03.dir/depend

