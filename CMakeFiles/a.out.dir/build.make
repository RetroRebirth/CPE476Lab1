# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.2

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
CMAKE_COMMAND = /Volumes/cmake-3.2.2-Darwin-x86_64/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Volumes/cmake-3.2.2-Darwin-x86_64/CMake.app/Contents/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/kayla/Projects/Graphics/CPE476Lab1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/kayla/Projects/Graphics/CPE476Lab1

# Include any dependencies generated for this target.
include CMakeFiles/a.out.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/a.out.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/a.out.dir/flags.make

CMakeFiles/a.out.dir/main.cpp.o: CMakeFiles/a.out.dir/flags.make
CMakeFiles/a.out.dir/main.cpp.o: main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/kayla/Projects/Graphics/CPE476Lab1/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/a.out.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/a.out.dir/main.cpp.o -c /Users/kayla/Projects/Graphics/CPE476Lab1/main.cpp

CMakeFiles/a.out.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/a.out.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/kayla/Projects/Graphics/CPE476Lab1/main.cpp > CMakeFiles/a.out.dir/main.cpp.i

CMakeFiles/a.out.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/a.out.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/kayla/Projects/Graphics/CPE476Lab1/main.cpp -o CMakeFiles/a.out.dir/main.cpp.s

CMakeFiles/a.out.dir/main.cpp.o.requires:
.PHONY : CMakeFiles/a.out.dir/main.cpp.o.requires

CMakeFiles/a.out.dir/main.cpp.o.provides: CMakeFiles/a.out.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/a.out.dir/build.make CMakeFiles/a.out.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/a.out.dir/main.cpp.o.provides

CMakeFiles/a.out.dir/main.cpp.o.provides.build: CMakeFiles/a.out.dir/main.cpp.o

CMakeFiles/a.out.dir/GLSL.cpp.o: CMakeFiles/a.out.dir/flags.make
CMakeFiles/a.out.dir/GLSL.cpp.o: GLSL.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/kayla/Projects/Graphics/CPE476Lab1/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/a.out.dir/GLSL.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/a.out.dir/GLSL.cpp.o -c /Users/kayla/Projects/Graphics/CPE476Lab1/GLSL.cpp

CMakeFiles/a.out.dir/GLSL.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/a.out.dir/GLSL.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/kayla/Projects/Graphics/CPE476Lab1/GLSL.cpp > CMakeFiles/a.out.dir/GLSL.cpp.i

CMakeFiles/a.out.dir/GLSL.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/a.out.dir/GLSL.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/kayla/Projects/Graphics/CPE476Lab1/GLSL.cpp -o CMakeFiles/a.out.dir/GLSL.cpp.s

CMakeFiles/a.out.dir/GLSL.cpp.o.requires:
.PHONY : CMakeFiles/a.out.dir/GLSL.cpp.o.requires

CMakeFiles/a.out.dir/GLSL.cpp.o.provides: CMakeFiles/a.out.dir/GLSL.cpp.o.requires
	$(MAKE) -f CMakeFiles/a.out.dir/build.make CMakeFiles/a.out.dir/GLSL.cpp.o.provides.build
.PHONY : CMakeFiles/a.out.dir/GLSL.cpp.o.provides

CMakeFiles/a.out.dir/GLSL.cpp.o.provides.build: CMakeFiles/a.out.dir/GLSL.cpp.o

CMakeFiles/a.out.dir/tiny_obj_loader.cc.o: CMakeFiles/a.out.dir/flags.make
CMakeFiles/a.out.dir/tiny_obj_loader.cc.o: tiny_obj_loader.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/kayla/Projects/Graphics/CPE476Lab1/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/a.out.dir/tiny_obj_loader.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/a.out.dir/tiny_obj_loader.cc.o -c /Users/kayla/Projects/Graphics/CPE476Lab1/tiny_obj_loader.cc

CMakeFiles/a.out.dir/tiny_obj_loader.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/a.out.dir/tiny_obj_loader.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/kayla/Projects/Graphics/CPE476Lab1/tiny_obj_loader.cc > CMakeFiles/a.out.dir/tiny_obj_loader.cc.i

CMakeFiles/a.out.dir/tiny_obj_loader.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/a.out.dir/tiny_obj_loader.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/kayla/Projects/Graphics/CPE476Lab1/tiny_obj_loader.cc -o CMakeFiles/a.out.dir/tiny_obj_loader.cc.s

CMakeFiles/a.out.dir/tiny_obj_loader.cc.o.requires:
.PHONY : CMakeFiles/a.out.dir/tiny_obj_loader.cc.o.requires

CMakeFiles/a.out.dir/tiny_obj_loader.cc.o.provides: CMakeFiles/a.out.dir/tiny_obj_loader.cc.o.requires
	$(MAKE) -f CMakeFiles/a.out.dir/build.make CMakeFiles/a.out.dir/tiny_obj_loader.cc.o.provides.build
.PHONY : CMakeFiles/a.out.dir/tiny_obj_loader.cc.o.provides

CMakeFiles/a.out.dir/tiny_obj_loader.cc.o.provides.build: CMakeFiles/a.out.dir/tiny_obj_loader.cc.o

CMakeFiles/a.out.dir/Booth.cpp.o: CMakeFiles/a.out.dir/flags.make
CMakeFiles/a.out.dir/Booth.cpp.o: Booth.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/kayla/Projects/Graphics/CPE476Lab1/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/a.out.dir/Booth.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/a.out.dir/Booth.cpp.o -c /Users/kayla/Projects/Graphics/CPE476Lab1/Booth.cpp

CMakeFiles/a.out.dir/Booth.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/a.out.dir/Booth.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/kayla/Projects/Graphics/CPE476Lab1/Booth.cpp > CMakeFiles/a.out.dir/Booth.cpp.i

CMakeFiles/a.out.dir/Booth.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/a.out.dir/Booth.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/kayla/Projects/Graphics/CPE476Lab1/Booth.cpp -o CMakeFiles/a.out.dir/Booth.cpp.s

CMakeFiles/a.out.dir/Booth.cpp.o.requires:
.PHONY : CMakeFiles/a.out.dir/Booth.cpp.o.requires

CMakeFiles/a.out.dir/Booth.cpp.o.provides: CMakeFiles/a.out.dir/Booth.cpp.o.requires
	$(MAKE) -f CMakeFiles/a.out.dir/build.make CMakeFiles/a.out.dir/Booth.cpp.o.provides.build
.PHONY : CMakeFiles/a.out.dir/Booth.cpp.o.provides

CMakeFiles/a.out.dir/Booth.cpp.o.provides.build: CMakeFiles/a.out.dir/Booth.cpp.o

CMakeFiles/a.out.dir/Camera.cpp.o: CMakeFiles/a.out.dir/flags.make
CMakeFiles/a.out.dir/Camera.cpp.o: Camera.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/kayla/Projects/Graphics/CPE476Lab1/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/a.out.dir/Camera.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/a.out.dir/Camera.cpp.o -c /Users/kayla/Projects/Graphics/CPE476Lab1/Camera.cpp

CMakeFiles/a.out.dir/Camera.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/a.out.dir/Camera.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/kayla/Projects/Graphics/CPE476Lab1/Camera.cpp > CMakeFiles/a.out.dir/Camera.cpp.i

CMakeFiles/a.out.dir/Camera.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/a.out.dir/Camera.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/kayla/Projects/Graphics/CPE476Lab1/Camera.cpp -o CMakeFiles/a.out.dir/Camera.cpp.s

CMakeFiles/a.out.dir/Camera.cpp.o.requires:
.PHONY : CMakeFiles/a.out.dir/Camera.cpp.o.requires

CMakeFiles/a.out.dir/Camera.cpp.o.provides: CMakeFiles/a.out.dir/Camera.cpp.o.requires
	$(MAKE) -f CMakeFiles/a.out.dir/build.make CMakeFiles/a.out.dir/Camera.cpp.o.provides.build
.PHONY : CMakeFiles/a.out.dir/Camera.cpp.o.provides

CMakeFiles/a.out.dir/Camera.cpp.o.provides.build: CMakeFiles/a.out.dir/Camera.cpp.o

CMakeFiles/a.out.dir/MatrixStack.cpp.o: CMakeFiles/a.out.dir/flags.make
CMakeFiles/a.out.dir/MatrixStack.cpp.o: MatrixStack.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/kayla/Projects/Graphics/CPE476Lab1/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/a.out.dir/MatrixStack.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/a.out.dir/MatrixStack.cpp.o -c /Users/kayla/Projects/Graphics/CPE476Lab1/MatrixStack.cpp

CMakeFiles/a.out.dir/MatrixStack.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/a.out.dir/MatrixStack.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/kayla/Projects/Graphics/CPE476Lab1/MatrixStack.cpp > CMakeFiles/a.out.dir/MatrixStack.cpp.i

CMakeFiles/a.out.dir/MatrixStack.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/a.out.dir/MatrixStack.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/kayla/Projects/Graphics/CPE476Lab1/MatrixStack.cpp -o CMakeFiles/a.out.dir/MatrixStack.cpp.s

CMakeFiles/a.out.dir/MatrixStack.cpp.o.requires:
.PHONY : CMakeFiles/a.out.dir/MatrixStack.cpp.o.requires

CMakeFiles/a.out.dir/MatrixStack.cpp.o.provides: CMakeFiles/a.out.dir/MatrixStack.cpp.o.requires
	$(MAKE) -f CMakeFiles/a.out.dir/build.make CMakeFiles/a.out.dir/MatrixStack.cpp.o.provides.build
.PHONY : CMakeFiles/a.out.dir/MatrixStack.cpp.o.provides

CMakeFiles/a.out.dir/MatrixStack.cpp.o.provides.build: CMakeFiles/a.out.dir/MatrixStack.cpp.o

CMakeFiles/a.out.dir/Object.cpp.o: CMakeFiles/a.out.dir/flags.make
CMakeFiles/a.out.dir/Object.cpp.o: Object.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/kayla/Projects/Graphics/CPE476Lab1/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/a.out.dir/Object.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/a.out.dir/Object.cpp.o -c /Users/kayla/Projects/Graphics/CPE476Lab1/Object.cpp

CMakeFiles/a.out.dir/Object.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/a.out.dir/Object.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/kayla/Projects/Graphics/CPE476Lab1/Object.cpp > CMakeFiles/a.out.dir/Object.cpp.i

CMakeFiles/a.out.dir/Object.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/a.out.dir/Object.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/kayla/Projects/Graphics/CPE476Lab1/Object.cpp -o CMakeFiles/a.out.dir/Object.cpp.s

CMakeFiles/a.out.dir/Object.cpp.o.requires:
.PHONY : CMakeFiles/a.out.dir/Object.cpp.o.requires

CMakeFiles/a.out.dir/Object.cpp.o.provides: CMakeFiles/a.out.dir/Object.cpp.o.requires
	$(MAKE) -f CMakeFiles/a.out.dir/build.make CMakeFiles/a.out.dir/Object.cpp.o.provides.build
.PHONY : CMakeFiles/a.out.dir/Object.cpp.o.provides

CMakeFiles/a.out.dir/Object.cpp.o.provides.build: CMakeFiles/a.out.dir/Object.cpp.o

CMakeFiles/a.out.dir/Player.cpp.o: CMakeFiles/a.out.dir/flags.make
CMakeFiles/a.out.dir/Player.cpp.o: Player.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/kayla/Projects/Graphics/CPE476Lab1/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/a.out.dir/Player.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/a.out.dir/Player.cpp.o -c /Users/kayla/Projects/Graphics/CPE476Lab1/Player.cpp

CMakeFiles/a.out.dir/Player.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/a.out.dir/Player.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/kayla/Projects/Graphics/CPE476Lab1/Player.cpp > CMakeFiles/a.out.dir/Player.cpp.i

CMakeFiles/a.out.dir/Player.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/a.out.dir/Player.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/kayla/Projects/Graphics/CPE476Lab1/Player.cpp -o CMakeFiles/a.out.dir/Player.cpp.s

CMakeFiles/a.out.dir/Player.cpp.o.requires:
.PHONY : CMakeFiles/a.out.dir/Player.cpp.o.requires

CMakeFiles/a.out.dir/Player.cpp.o.provides: CMakeFiles/a.out.dir/Player.cpp.o.requires
	$(MAKE) -f CMakeFiles/a.out.dir/build.make CMakeFiles/a.out.dir/Player.cpp.o.provides.build
.PHONY : CMakeFiles/a.out.dir/Player.cpp.o.provides

CMakeFiles/a.out.dir/Player.cpp.o.provides.build: CMakeFiles/a.out.dir/Player.cpp.o

CMakeFiles/a.out.dir/Session.cpp.o: CMakeFiles/a.out.dir/flags.make
CMakeFiles/a.out.dir/Session.cpp.o: Session.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/kayla/Projects/Graphics/CPE476Lab1/CMakeFiles $(CMAKE_PROGRESS_9)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/a.out.dir/Session.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/a.out.dir/Session.cpp.o -c /Users/kayla/Projects/Graphics/CPE476Lab1/Session.cpp

CMakeFiles/a.out.dir/Session.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/a.out.dir/Session.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/kayla/Projects/Graphics/CPE476Lab1/Session.cpp > CMakeFiles/a.out.dir/Session.cpp.i

CMakeFiles/a.out.dir/Session.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/a.out.dir/Session.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/kayla/Projects/Graphics/CPE476Lab1/Session.cpp -o CMakeFiles/a.out.dir/Session.cpp.s

CMakeFiles/a.out.dir/Session.cpp.o.requires:
.PHONY : CMakeFiles/a.out.dir/Session.cpp.o.requires

CMakeFiles/a.out.dir/Session.cpp.o.provides: CMakeFiles/a.out.dir/Session.cpp.o.requires
	$(MAKE) -f CMakeFiles/a.out.dir/build.make CMakeFiles/a.out.dir/Session.cpp.o.provides.build
.PHONY : CMakeFiles/a.out.dir/Session.cpp.o.provides

CMakeFiles/a.out.dir/Session.cpp.o.provides.build: CMakeFiles/a.out.dir/Session.cpp.o

CMakeFiles/a.out.dir/SkyBox.cpp.o: CMakeFiles/a.out.dir/flags.make
CMakeFiles/a.out.dir/SkyBox.cpp.o: SkyBox.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/kayla/Projects/Graphics/CPE476Lab1/CMakeFiles $(CMAKE_PROGRESS_10)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/a.out.dir/SkyBox.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/a.out.dir/SkyBox.cpp.o -c /Users/kayla/Projects/Graphics/CPE476Lab1/SkyBox.cpp

CMakeFiles/a.out.dir/SkyBox.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/a.out.dir/SkyBox.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/kayla/Projects/Graphics/CPE476Lab1/SkyBox.cpp > CMakeFiles/a.out.dir/SkyBox.cpp.i

CMakeFiles/a.out.dir/SkyBox.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/a.out.dir/SkyBox.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/kayla/Projects/Graphics/CPE476Lab1/SkyBox.cpp -o CMakeFiles/a.out.dir/SkyBox.cpp.s

CMakeFiles/a.out.dir/SkyBox.cpp.o.requires:
.PHONY : CMakeFiles/a.out.dir/SkyBox.cpp.o.requires

CMakeFiles/a.out.dir/SkyBox.cpp.o.provides: CMakeFiles/a.out.dir/SkyBox.cpp.o.requires
	$(MAKE) -f CMakeFiles/a.out.dir/build.make CMakeFiles/a.out.dir/SkyBox.cpp.o.provides.build
.PHONY : CMakeFiles/a.out.dir/SkyBox.cpp.o.provides

CMakeFiles/a.out.dir/SkyBox.cpp.o.provides.build: CMakeFiles/a.out.dir/SkyBox.cpp.o

CMakeFiles/a.out.dir/Window.cpp.o: CMakeFiles/a.out.dir/flags.make
CMakeFiles/a.out.dir/Window.cpp.o: Window.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/kayla/Projects/Graphics/CPE476Lab1/CMakeFiles $(CMAKE_PROGRESS_11)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/a.out.dir/Window.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/a.out.dir/Window.cpp.o -c /Users/kayla/Projects/Graphics/CPE476Lab1/Window.cpp

CMakeFiles/a.out.dir/Window.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/a.out.dir/Window.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/kayla/Projects/Graphics/CPE476Lab1/Window.cpp > CMakeFiles/a.out.dir/Window.cpp.i

CMakeFiles/a.out.dir/Window.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/a.out.dir/Window.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/kayla/Projects/Graphics/CPE476Lab1/Window.cpp -o CMakeFiles/a.out.dir/Window.cpp.s

CMakeFiles/a.out.dir/Window.cpp.o.requires:
.PHONY : CMakeFiles/a.out.dir/Window.cpp.o.requires

CMakeFiles/a.out.dir/Window.cpp.o.provides: CMakeFiles/a.out.dir/Window.cpp.o.requires
	$(MAKE) -f CMakeFiles/a.out.dir/build.make CMakeFiles/a.out.dir/Window.cpp.o.provides.build
.PHONY : CMakeFiles/a.out.dir/Window.cpp.o.provides

CMakeFiles/a.out.dir/Window.cpp.o.provides.build: CMakeFiles/a.out.dir/Window.cpp.o

CMakeFiles/a.out.dir/World.cpp.o: CMakeFiles/a.out.dir/flags.make
CMakeFiles/a.out.dir/World.cpp.o: World.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/kayla/Projects/Graphics/CPE476Lab1/CMakeFiles $(CMAKE_PROGRESS_12)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/a.out.dir/World.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/a.out.dir/World.cpp.o -c /Users/kayla/Projects/Graphics/CPE476Lab1/World.cpp

CMakeFiles/a.out.dir/World.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/a.out.dir/World.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/kayla/Projects/Graphics/CPE476Lab1/World.cpp > CMakeFiles/a.out.dir/World.cpp.i

CMakeFiles/a.out.dir/World.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/a.out.dir/World.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/kayla/Projects/Graphics/CPE476Lab1/World.cpp -o CMakeFiles/a.out.dir/World.cpp.s

CMakeFiles/a.out.dir/World.cpp.o.requires:
.PHONY : CMakeFiles/a.out.dir/World.cpp.o.requires

CMakeFiles/a.out.dir/World.cpp.o.provides: CMakeFiles/a.out.dir/World.cpp.o.requires
	$(MAKE) -f CMakeFiles/a.out.dir/build.make CMakeFiles/a.out.dir/World.cpp.o.provides.build
.PHONY : CMakeFiles/a.out.dir/World.cpp.o.provides

CMakeFiles/a.out.dir/World.cpp.o.provides.build: CMakeFiles/a.out.dir/World.cpp.o

CMakeFiles/a.out.dir/Texture.cpp.o: CMakeFiles/a.out.dir/flags.make
CMakeFiles/a.out.dir/Texture.cpp.o: Texture.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/kayla/Projects/Graphics/CPE476Lab1/CMakeFiles $(CMAKE_PROGRESS_13)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/a.out.dir/Texture.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/a.out.dir/Texture.cpp.o -c /Users/kayla/Projects/Graphics/CPE476Lab1/Texture.cpp

CMakeFiles/a.out.dir/Texture.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/a.out.dir/Texture.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/kayla/Projects/Graphics/CPE476Lab1/Texture.cpp > CMakeFiles/a.out.dir/Texture.cpp.i

CMakeFiles/a.out.dir/Texture.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/a.out.dir/Texture.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/kayla/Projects/Graphics/CPE476Lab1/Texture.cpp -o CMakeFiles/a.out.dir/Texture.cpp.s

CMakeFiles/a.out.dir/Texture.cpp.o.requires:
.PHONY : CMakeFiles/a.out.dir/Texture.cpp.o.requires

CMakeFiles/a.out.dir/Texture.cpp.o.provides: CMakeFiles/a.out.dir/Texture.cpp.o.requires
	$(MAKE) -f CMakeFiles/a.out.dir/build.make CMakeFiles/a.out.dir/Texture.cpp.o.provides.build
.PHONY : CMakeFiles/a.out.dir/Texture.cpp.o.provides

CMakeFiles/a.out.dir/Texture.cpp.o.provides.build: CMakeFiles/a.out.dir/Texture.cpp.o

CMakeFiles/a.out.dir/Clicks.cpp.o: CMakeFiles/a.out.dir/flags.make
CMakeFiles/a.out.dir/Clicks.cpp.o: Clicks.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/kayla/Projects/Graphics/CPE476Lab1/CMakeFiles $(CMAKE_PROGRESS_14)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/a.out.dir/Clicks.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/a.out.dir/Clicks.cpp.o -c /Users/kayla/Projects/Graphics/CPE476Lab1/Clicks.cpp

CMakeFiles/a.out.dir/Clicks.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/a.out.dir/Clicks.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/kayla/Projects/Graphics/CPE476Lab1/Clicks.cpp > CMakeFiles/a.out.dir/Clicks.cpp.i

CMakeFiles/a.out.dir/Clicks.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/a.out.dir/Clicks.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/kayla/Projects/Graphics/CPE476Lab1/Clicks.cpp -o CMakeFiles/a.out.dir/Clicks.cpp.s

CMakeFiles/a.out.dir/Clicks.cpp.o.requires:
.PHONY : CMakeFiles/a.out.dir/Clicks.cpp.o.requires

CMakeFiles/a.out.dir/Clicks.cpp.o.provides: CMakeFiles/a.out.dir/Clicks.cpp.o.requires
	$(MAKE) -f CMakeFiles/a.out.dir/build.make CMakeFiles/a.out.dir/Clicks.cpp.o.provides.build
.PHONY : CMakeFiles/a.out.dir/Clicks.cpp.o.provides

CMakeFiles/a.out.dir/Clicks.cpp.o.provides.build: CMakeFiles/a.out.dir/Clicks.cpp.o

CMakeFiles/a.out.dir/ShootingGallery.cpp.o: CMakeFiles/a.out.dir/flags.make
CMakeFiles/a.out.dir/ShootingGallery.cpp.o: ShootingGallery.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/kayla/Projects/Graphics/CPE476Lab1/CMakeFiles $(CMAKE_PROGRESS_15)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/a.out.dir/ShootingGallery.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/a.out.dir/ShootingGallery.cpp.o -c /Users/kayla/Projects/Graphics/CPE476Lab1/ShootingGallery.cpp

CMakeFiles/a.out.dir/ShootingGallery.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/a.out.dir/ShootingGallery.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/kayla/Projects/Graphics/CPE476Lab1/ShootingGallery.cpp > CMakeFiles/a.out.dir/ShootingGallery.cpp.i

CMakeFiles/a.out.dir/ShootingGallery.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/a.out.dir/ShootingGallery.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/kayla/Projects/Graphics/CPE476Lab1/ShootingGallery.cpp -o CMakeFiles/a.out.dir/ShootingGallery.cpp.s

CMakeFiles/a.out.dir/ShootingGallery.cpp.o.requires:
.PHONY : CMakeFiles/a.out.dir/ShootingGallery.cpp.o.requires

CMakeFiles/a.out.dir/ShootingGallery.cpp.o.provides: CMakeFiles/a.out.dir/ShootingGallery.cpp.o.requires
	$(MAKE) -f CMakeFiles/a.out.dir/build.make CMakeFiles/a.out.dir/ShootingGallery.cpp.o.provides.build
.PHONY : CMakeFiles/a.out.dir/ShootingGallery.cpp.o.provides

CMakeFiles/a.out.dir/ShootingGallery.cpp.o.provides.build: CMakeFiles/a.out.dir/ShootingGallery.cpp.o

CMakeFiles/a.out.dir/Sound.cpp.o: CMakeFiles/a.out.dir/flags.make
CMakeFiles/a.out.dir/Sound.cpp.o: Sound.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/kayla/Projects/Graphics/CPE476Lab1/CMakeFiles $(CMAKE_PROGRESS_16)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/a.out.dir/Sound.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/a.out.dir/Sound.cpp.o -c /Users/kayla/Projects/Graphics/CPE476Lab1/Sound.cpp

CMakeFiles/a.out.dir/Sound.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/a.out.dir/Sound.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/kayla/Projects/Graphics/CPE476Lab1/Sound.cpp > CMakeFiles/a.out.dir/Sound.cpp.i

CMakeFiles/a.out.dir/Sound.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/a.out.dir/Sound.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/kayla/Projects/Graphics/CPE476Lab1/Sound.cpp -o CMakeFiles/a.out.dir/Sound.cpp.s

CMakeFiles/a.out.dir/Sound.cpp.o.requires:
.PHONY : CMakeFiles/a.out.dir/Sound.cpp.o.requires

CMakeFiles/a.out.dir/Sound.cpp.o.provides: CMakeFiles/a.out.dir/Sound.cpp.o.requires
	$(MAKE) -f CMakeFiles/a.out.dir/build.make CMakeFiles/a.out.dir/Sound.cpp.o.provides.build
.PHONY : CMakeFiles/a.out.dir/Sound.cpp.o.provides

CMakeFiles/a.out.dir/Sound.cpp.o.provides.build: CMakeFiles/a.out.dir/Sound.cpp.o

# Object files for target a.out
a_out_OBJECTS = \
"CMakeFiles/a.out.dir/main.cpp.o" \
"CMakeFiles/a.out.dir/GLSL.cpp.o" \
"CMakeFiles/a.out.dir/tiny_obj_loader.cc.o" \
"CMakeFiles/a.out.dir/Booth.cpp.o" \
"CMakeFiles/a.out.dir/Camera.cpp.o" \
"CMakeFiles/a.out.dir/MatrixStack.cpp.o" \
"CMakeFiles/a.out.dir/Object.cpp.o" \
"CMakeFiles/a.out.dir/Player.cpp.o" \
"CMakeFiles/a.out.dir/Session.cpp.o" \
"CMakeFiles/a.out.dir/SkyBox.cpp.o" \
"CMakeFiles/a.out.dir/Window.cpp.o" \
"CMakeFiles/a.out.dir/World.cpp.o" \
"CMakeFiles/a.out.dir/Texture.cpp.o" \
"CMakeFiles/a.out.dir/Clicks.cpp.o" \
"CMakeFiles/a.out.dir/ShootingGallery.cpp.o" \
"CMakeFiles/a.out.dir/Sound.cpp.o"

# External object files for target a.out
a_out_EXTERNAL_OBJECTS =

a.out: CMakeFiles/a.out.dir/main.cpp.o
a.out: CMakeFiles/a.out.dir/GLSL.cpp.o
a.out: CMakeFiles/a.out.dir/tiny_obj_loader.cc.o
a.out: CMakeFiles/a.out.dir/Booth.cpp.o
a.out: CMakeFiles/a.out.dir/Camera.cpp.o
a.out: CMakeFiles/a.out.dir/MatrixStack.cpp.o
a.out: CMakeFiles/a.out.dir/Object.cpp.o
a.out: CMakeFiles/a.out.dir/Player.cpp.o
a.out: CMakeFiles/a.out.dir/Session.cpp.o
a.out: CMakeFiles/a.out.dir/SkyBox.cpp.o
a.out: CMakeFiles/a.out.dir/Window.cpp.o
a.out: CMakeFiles/a.out.dir/World.cpp.o
a.out: CMakeFiles/a.out.dir/Texture.cpp.o
a.out: CMakeFiles/a.out.dir/Clicks.cpp.o
a.out: CMakeFiles/a.out.dir/ShootingGallery.cpp.o
a.out: CMakeFiles/a.out.dir/Sound.cpp.o
a.out: CMakeFiles/a.out.dir/build.make
a.out: /usr/local/lib/libassimp.dylib
a.out: /usr/local/lib/libglfw3.dylib
a.out: /usr/local/lib/libGLEW.dylib
a.out: irrKlang-64bit-1.5.0/bin/macosx-gcc/libirrklang.dylib
a.out: /usr/local/lib/libikpMP3.dylib
a.out: CMakeFiles/a.out.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable a.out"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/a.out.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/a.out.dir/build: a.out
.PHONY : CMakeFiles/a.out.dir/build

CMakeFiles/a.out.dir/requires: CMakeFiles/a.out.dir/main.cpp.o.requires
CMakeFiles/a.out.dir/requires: CMakeFiles/a.out.dir/GLSL.cpp.o.requires
CMakeFiles/a.out.dir/requires: CMakeFiles/a.out.dir/tiny_obj_loader.cc.o.requires
CMakeFiles/a.out.dir/requires: CMakeFiles/a.out.dir/Booth.cpp.o.requires
CMakeFiles/a.out.dir/requires: CMakeFiles/a.out.dir/Camera.cpp.o.requires
CMakeFiles/a.out.dir/requires: CMakeFiles/a.out.dir/MatrixStack.cpp.o.requires
CMakeFiles/a.out.dir/requires: CMakeFiles/a.out.dir/Object.cpp.o.requires
CMakeFiles/a.out.dir/requires: CMakeFiles/a.out.dir/Player.cpp.o.requires
CMakeFiles/a.out.dir/requires: CMakeFiles/a.out.dir/Session.cpp.o.requires
CMakeFiles/a.out.dir/requires: CMakeFiles/a.out.dir/SkyBox.cpp.o.requires
CMakeFiles/a.out.dir/requires: CMakeFiles/a.out.dir/Window.cpp.o.requires
CMakeFiles/a.out.dir/requires: CMakeFiles/a.out.dir/World.cpp.o.requires
CMakeFiles/a.out.dir/requires: CMakeFiles/a.out.dir/Texture.cpp.o.requires
CMakeFiles/a.out.dir/requires: CMakeFiles/a.out.dir/Clicks.cpp.o.requires
CMakeFiles/a.out.dir/requires: CMakeFiles/a.out.dir/ShootingGallery.cpp.o.requires
CMakeFiles/a.out.dir/requires: CMakeFiles/a.out.dir/Sound.cpp.o.requires
.PHONY : CMakeFiles/a.out.dir/requires

CMakeFiles/a.out.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/a.out.dir/cmake_clean.cmake
.PHONY : CMakeFiles/a.out.dir/clean

CMakeFiles/a.out.dir/depend:
	cd /Users/kayla/Projects/Graphics/CPE476Lab1 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/kayla/Projects/Graphics/CPE476Lab1 /Users/kayla/Projects/Graphics/CPE476Lab1 /Users/kayla/Projects/Graphics/CPE476Lab1 /Users/kayla/Projects/Graphics/CPE476Lab1 /Users/kayla/Projects/Graphics/CPE476Lab1/CMakeFiles/a.out.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/a.out.dir/depend

