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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.27.7/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.27.7/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/mainlarbibenamour/desktop/password_manager-2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/mainlarbibenamour/desktop/password_manager-2/build

# Include any dependencies generated for this target.
include CMakeFiles/password_manager.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/password_manager.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/password_manager.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/password_manager.dir/flags.make

CMakeFiles/password_manager.dir/main.cpp.o: CMakeFiles/password_manager.dir/flags.make
CMakeFiles/password_manager.dir/main.cpp.o: /Users/mainlarbibenamour/desktop/password_manager-2/main.cpp
CMakeFiles/password_manager.dir/main.cpp.o: CMakeFiles/password_manager.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/mainlarbibenamour/desktop/password_manager-2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/password_manager.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/password_manager.dir/main.cpp.o -MF CMakeFiles/password_manager.dir/main.cpp.o.d -o CMakeFiles/password_manager.dir/main.cpp.o -c /Users/mainlarbibenamour/desktop/password_manager-2/main.cpp

CMakeFiles/password_manager.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/password_manager.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mainlarbibenamour/desktop/password_manager-2/main.cpp > CMakeFiles/password_manager.dir/main.cpp.i

CMakeFiles/password_manager.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/password_manager.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mainlarbibenamour/desktop/password_manager-2/main.cpp -o CMakeFiles/password_manager.dir/main.cpp.s

CMakeFiles/password_manager.dir/manager.cpp.o: CMakeFiles/password_manager.dir/flags.make
CMakeFiles/password_manager.dir/manager.cpp.o: /Users/mainlarbibenamour/desktop/password_manager-2/manager.cpp
CMakeFiles/password_manager.dir/manager.cpp.o: CMakeFiles/password_manager.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/mainlarbibenamour/desktop/password_manager-2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/password_manager.dir/manager.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/password_manager.dir/manager.cpp.o -MF CMakeFiles/password_manager.dir/manager.cpp.o.d -o CMakeFiles/password_manager.dir/manager.cpp.o -c /Users/mainlarbibenamour/desktop/password_manager-2/manager.cpp

CMakeFiles/password_manager.dir/manager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/password_manager.dir/manager.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mainlarbibenamour/desktop/password_manager-2/manager.cpp > CMakeFiles/password_manager.dir/manager.cpp.i

CMakeFiles/password_manager.dir/manager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/password_manager.dir/manager.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mainlarbibenamour/desktop/password_manager-2/manager.cpp -o CMakeFiles/password_manager.dir/manager.cpp.s

# Object files for target password_manager
password_manager_OBJECTS = \
"CMakeFiles/password_manager.dir/main.cpp.o" \
"CMakeFiles/password_manager.dir/manager.cpp.o"

# External object files for target password_manager
password_manager_EXTERNAL_OBJECTS =

password_manager: CMakeFiles/password_manager.dir/main.cpp.o
password_manager: CMakeFiles/password_manager.dir/manager.cpp.o
password_manager: CMakeFiles/password_manager.dir/build.make
password_manager: /opt/homebrew/lib/libssl.dylib
password_manager: /opt/homebrew/lib/libcrypto.dylib
password_manager: CMakeFiles/password_manager.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/mainlarbibenamour/desktop/password_manager-2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable password_manager"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/password_manager.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/password_manager.dir/build: password_manager
.PHONY : CMakeFiles/password_manager.dir/build

CMakeFiles/password_manager.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/password_manager.dir/cmake_clean.cmake
.PHONY : CMakeFiles/password_manager.dir/clean

CMakeFiles/password_manager.dir/depend:
	cd /Users/mainlarbibenamour/desktop/password_manager-2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/mainlarbibenamour/desktop/password_manager-2 /Users/mainlarbibenamour/desktop/password_manager-2 /Users/mainlarbibenamour/desktop/password_manager-2/build /Users/mainlarbibenamour/desktop/password_manager-2/build /Users/mainlarbibenamour/desktop/password_manager-2/build/CMakeFiles/password_manager.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/password_manager.dir/depend
