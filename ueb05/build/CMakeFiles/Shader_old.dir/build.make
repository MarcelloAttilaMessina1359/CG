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
CMAKE_SOURCE_DIR = /home/marcello/Uni/cg1_12/ueb05

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/marcello/Uni/cg1_12/ueb05/build

# Include any dependencies generated for this target.
include CMakeFiles/Shader_old.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Shader_old.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Shader_old.dir/flags.make

CMakeFiles/Shader_old.dir/src/debugGL.c.o: CMakeFiles/Shader_old.dir/flags.make
CMakeFiles/Shader_old.dir/src/debugGL.c.o: ../src/debugGL.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marcello/Uni/cg1_12/ueb05/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Shader_old.dir/src/debugGL.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Shader_old.dir/src/debugGL.c.o   -c /home/marcello/Uni/cg1_12/ueb05/src/debugGL.c

CMakeFiles/Shader_old.dir/src/debugGL.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Shader_old.dir/src/debugGL.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/marcello/Uni/cg1_12/ueb05/src/debugGL.c > CMakeFiles/Shader_old.dir/src/debugGL.c.i

CMakeFiles/Shader_old.dir/src/debugGL.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Shader_old.dir/src/debugGL.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/marcello/Uni/cg1_12/ueb05/src/debugGL.c -o CMakeFiles/Shader_old.dir/src/debugGL.c.s

CMakeFiles/Shader_old.dir/src/shader.c.o: CMakeFiles/Shader_old.dir/flags.make
CMakeFiles/Shader_old.dir/src/shader.c.o: ../src/shader.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marcello/Uni/cg1_12/ueb05/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/Shader_old.dir/src/shader.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Shader_old.dir/src/shader.c.o   -c /home/marcello/Uni/cg1_12/ueb05/src/shader.c

CMakeFiles/Shader_old.dir/src/shader.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Shader_old.dir/src/shader.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/marcello/Uni/cg1_12/ueb05/src/shader.c > CMakeFiles/Shader_old.dir/src/shader.c.i

CMakeFiles/Shader_old.dir/src/shader.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Shader_old.dir/src/shader.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/marcello/Uni/cg1_12/ueb05/src/shader.c -o CMakeFiles/Shader_old.dir/src/shader.c.s

CMakeFiles/Shader_old.dir/src/utility.c.o: CMakeFiles/Shader_old.dir/flags.make
CMakeFiles/Shader_old.dir/src/utility.c.o: ../src/utility.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marcello/Uni/cg1_12/ueb05/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/Shader_old.dir/src/utility.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Shader_old.dir/src/utility.c.o   -c /home/marcello/Uni/cg1_12/ueb05/src/utility.c

CMakeFiles/Shader_old.dir/src/utility.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Shader_old.dir/src/utility.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/marcello/Uni/cg1_12/ueb05/src/utility.c > CMakeFiles/Shader_old.dir/src/utility.c.i

CMakeFiles/Shader_old.dir/src/utility.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Shader_old.dir/src/utility.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/marcello/Uni/cg1_12/ueb05/src/utility.c -o CMakeFiles/Shader_old.dir/src/utility.c.s

# Object files for target Shader_old
Shader_old_OBJECTS = \
"CMakeFiles/Shader_old.dir/src/debugGL.c.o" \
"CMakeFiles/Shader_old.dir/src/shader.c.o" \
"CMakeFiles/Shader_old.dir/src/utility.c.o"

# External object files for target Shader_old
Shader_old_EXTERNAL_OBJECTS =

Shader_old: CMakeFiles/Shader_old.dir/src/debugGL.c.o
Shader_old: CMakeFiles/Shader_old.dir/src/shader.c.o
Shader_old: CMakeFiles/Shader_old.dir/src/utility.c.o
Shader_old: CMakeFiles/Shader_old.dir/build.make
Shader_old: /usr/lib/x86_64-linux-gnu/libGL.so
Shader_old: /usr/lib/x86_64-linux-gnu/libGLEW.so
Shader_old: CMakeFiles/Shader_old.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/marcello/Uni/cg1_12/ueb05/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable Shader_old"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Shader_old.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Shader_old.dir/build: Shader_old

.PHONY : CMakeFiles/Shader_old.dir/build

CMakeFiles/Shader_old.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Shader_old.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Shader_old.dir/clean

CMakeFiles/Shader_old.dir/depend:
	cd /home/marcello/Uni/cg1_12/ueb05/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/marcello/Uni/cg1_12/ueb05 /home/marcello/Uni/cg1_12/ueb05 /home/marcello/Uni/cg1_12/ueb05/build /home/marcello/Uni/cg1_12/ueb05/build /home/marcello/Uni/cg1_12/ueb05/build/CMakeFiles/Shader_old.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Shader_old.dir/depend

