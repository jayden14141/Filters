# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.28.3/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.28.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/jayden/OneDrive - University of Bristol/Yr3/2/diss/filters"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/jayden/OneDrive - University of Bristol/Yr3/2/diss/filters/build"

# Include any dependencies generated for this target.
include tests/CMakeFiles/test_executable.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tests/CMakeFiles/test_executable.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/test_executable.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/test_executable.dir/flags.make

tests/CMakeFiles/test_executable.dir/test.cpp.o: tests/CMakeFiles/test_executable.dir/flags.make
tests/CMakeFiles/test_executable.dir/test.cpp.o: /Users/jayden/OneDrive\ -\ University\ of\ Bristol/Yr3/2/diss/filters/tests/test.cpp
tests/CMakeFiles/test_executable.dir/test.cpp.o: tests/CMakeFiles/test_executable.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/jayden/OneDrive - University of Bristol/Yr3/2/diss/filters/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/test_executable.dir/test.cpp.o"
	cd "/Users/jayden/OneDrive - University of Bristol/Yr3/2/diss/filters/build/tests" && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/test_executable.dir/test.cpp.o -MF CMakeFiles/test_executable.dir/test.cpp.o.d -o CMakeFiles/test_executable.dir/test.cpp.o -c "/Users/jayden/OneDrive - University of Bristol/Yr3/2/diss/filters/tests/test.cpp"

tests/CMakeFiles/test_executable.dir/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test_executable.dir/test.cpp.i"
	cd "/Users/jayden/OneDrive - University of Bristol/Yr3/2/diss/filters/build/tests" && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/jayden/OneDrive - University of Bristol/Yr3/2/diss/filters/tests/test.cpp" > CMakeFiles/test_executable.dir/test.cpp.i

tests/CMakeFiles/test_executable.dir/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test_executable.dir/test.cpp.s"
	cd "/Users/jayden/OneDrive - University of Bristol/Yr3/2/diss/filters/build/tests" && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/jayden/OneDrive - University of Bristol/Yr3/2/diss/filters/tests/test.cpp" -o CMakeFiles/test_executable.dir/test.cpp.s

# Object files for target test_executable
test_executable_OBJECTS = \
"CMakeFiles/test_executable.dir/test.cpp.o"

# External object files for target test_executable
test_executable_EXTERNAL_OBJECTS =

tests/test_executable: tests/CMakeFiles/test_executable.dir/test.cpp.o
tests/test_executable: tests/CMakeFiles/test_executable.dir/build.make
tests/test_executable: libfilters.a
tests/test_executable: tests/CMakeFiles/test_executable.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/Users/jayden/OneDrive - University of Bristol/Yr3/2/diss/filters/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test_executable"
	cd "/Users/jayden/OneDrive - University of Bristol/Yr3/2/diss/filters/build/tests" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_executable.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/test_executable.dir/build: tests/test_executable
.PHONY : tests/CMakeFiles/test_executable.dir/build

tests/CMakeFiles/test_executable.dir/clean:
	cd "/Users/jayden/OneDrive - University of Bristol/Yr3/2/diss/filters/build/tests" && $(CMAKE_COMMAND) -P CMakeFiles/test_executable.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/test_executable.dir/clean

tests/CMakeFiles/test_executable.dir/depend:
	cd "/Users/jayden/OneDrive - University of Bristol/Yr3/2/diss/filters/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/jayden/OneDrive - University of Bristol/Yr3/2/diss/filters" "/Users/jayden/OneDrive - University of Bristol/Yr3/2/diss/filters/tests" "/Users/jayden/OneDrive - University of Bristol/Yr3/2/diss/filters/build" "/Users/jayden/OneDrive - University of Bristol/Yr3/2/diss/filters/build/tests" "/Users/jayden/OneDrive - University of Bristol/Yr3/2/diss/filters/build/tests/CMakeFiles/test_executable.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : tests/CMakeFiles/test_executable.dir/depend

