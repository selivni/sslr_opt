# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mark/programming/diploma

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mark/programming/diploma/build

# Include any dependencies generated for this target.
include CMakeFiles/sslr_opt.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/sslr_opt.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sslr_opt.dir/flags.make

CMakeFiles/sslr_opt.dir/source/SceneLoader.cpp.o: CMakeFiles/sslr_opt.dir/flags.make
CMakeFiles/sslr_opt.dir/source/SceneLoader.cpp.o: ../source/SceneLoader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mark/programming/diploma/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/sslr_opt.dir/source/SceneLoader.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sslr_opt.dir/source/SceneLoader.cpp.o -c /home/mark/programming/diploma/source/SceneLoader.cpp

CMakeFiles/sslr_opt.dir/source/SceneLoader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sslr_opt.dir/source/SceneLoader.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mark/programming/diploma/source/SceneLoader.cpp > CMakeFiles/sslr_opt.dir/source/SceneLoader.cpp.i

CMakeFiles/sslr_opt.dir/source/SceneLoader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sslr_opt.dir/source/SceneLoader.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mark/programming/diploma/source/SceneLoader.cpp -o CMakeFiles/sslr_opt.dir/source/SceneLoader.cpp.s

CMakeFiles/sslr_opt.dir/source/SceneLoader.cpp.o.requires:

.PHONY : CMakeFiles/sslr_opt.dir/source/SceneLoader.cpp.o.requires

CMakeFiles/sslr_opt.dir/source/SceneLoader.cpp.o.provides: CMakeFiles/sslr_opt.dir/source/SceneLoader.cpp.o.requires
	$(MAKE) -f CMakeFiles/sslr_opt.dir/build.make CMakeFiles/sslr_opt.dir/source/SceneLoader.cpp.o.provides.build
.PHONY : CMakeFiles/sslr_opt.dir/source/SceneLoader.cpp.o.provides

CMakeFiles/sslr_opt.dir/source/SceneLoader.cpp.o.provides.build: CMakeFiles/sslr_opt.dir/source/SceneLoader.cpp.o


CMakeFiles/sslr_opt.dir/source/main.cpp.o: CMakeFiles/sslr_opt.dir/flags.make
CMakeFiles/sslr_opt.dir/source/main.cpp.o: ../source/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mark/programming/diploma/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/sslr_opt.dir/source/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sslr_opt.dir/source/main.cpp.o -c /home/mark/programming/diploma/source/main.cpp

CMakeFiles/sslr_opt.dir/source/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sslr_opt.dir/source/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mark/programming/diploma/source/main.cpp > CMakeFiles/sslr_opt.dir/source/main.cpp.i

CMakeFiles/sslr_opt.dir/source/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sslr_opt.dir/source/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mark/programming/diploma/source/main.cpp -o CMakeFiles/sslr_opt.dir/source/main.cpp.s

CMakeFiles/sslr_opt.dir/source/main.cpp.o.requires:

.PHONY : CMakeFiles/sslr_opt.dir/source/main.cpp.o.requires

CMakeFiles/sslr_opt.dir/source/main.cpp.o.provides: CMakeFiles/sslr_opt.dir/source/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/sslr_opt.dir/build.make CMakeFiles/sslr_opt.dir/source/main.cpp.o.provides.build
.PHONY : CMakeFiles/sslr_opt.dir/source/main.cpp.o.provides

CMakeFiles/sslr_opt.dir/source/main.cpp.o.provides.build: CMakeFiles/sslr_opt.dir/source/main.cpp.o


# Object files for target sslr_opt
sslr_opt_OBJECTS = \
"CMakeFiles/sslr_opt.dir/source/SceneLoader.cpp.o" \
"CMakeFiles/sslr_opt.dir/source/main.cpp.o"

# External object files for target sslr_opt
sslr_opt_EXTERNAL_OBJECTS =

sslr_opt: CMakeFiles/sslr_opt.dir/source/SceneLoader.cpp.o
sslr_opt: CMakeFiles/sslr_opt.dir/source/main.cpp.o
sslr_opt: CMakeFiles/sslr_opt.dir/build.make
sslr_opt: ../shared/assimp/libassimp.so
sslr_opt: ../lib/Utility/obj/Camera.o
sslr_opt: ../lib/Utility/obj/GL.o
sslr_opt: ../lib/Utility/obj/Overall.o
sslr_opt: ../lib/Utility/obj/ShaderProgram.o
sslr_opt: ../lib/Utility/obj/Texture.o
sslr_opt: ../lib/Utility/obj/mat4.o
sslr_opt: ../lib/Utility/obj/uvec3.o
sslr_opt: ../lib/Utility/obj/vec2.o
sslr_opt: ../lib/Utility/obj/vec3.o
sslr_opt: ../lib/Utility/obj/vec4.o
sslr_opt: ../lib/SOIL/obj/SOIL.o
sslr_opt: ../lib/SOIL/obj/image_DXT.o
sslr_opt: ../lib/SOIL/obj/image_helper.o
sslr_opt: ../lib/SOIL/obj/stb_image_aug.o
sslr_opt: /usr/lib/x86_64-linux-gnu/libGLEW.so
sslr_opt: /usr/lib/x86_64-linux-gnu/libglut.so
sslr_opt: /usr/lib/x86_64-linux-gnu/libGLU.so
sslr_opt: /usr/lib/x86_64-linux-gnu/libGL.so
sslr_opt: CMakeFiles/sslr_opt.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mark/programming/diploma/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable sslr_opt"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sslr_opt.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sslr_opt.dir/build: sslr_opt

.PHONY : CMakeFiles/sslr_opt.dir/build

CMakeFiles/sslr_opt.dir/requires: CMakeFiles/sslr_opt.dir/source/SceneLoader.cpp.o.requires
CMakeFiles/sslr_opt.dir/requires: CMakeFiles/sslr_opt.dir/source/main.cpp.o.requires

.PHONY : CMakeFiles/sslr_opt.dir/requires

CMakeFiles/sslr_opt.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sslr_opt.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sslr_opt.dir/clean

CMakeFiles/sslr_opt.dir/depend:
	cd /home/mark/programming/diploma/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mark/programming/diploma /home/mark/programming/diploma /home/mark/programming/diploma/build /home/mark/programming/diploma/build /home/mark/programming/diploma/build/CMakeFiles/sslr_opt.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sslr_opt.dir/depend

