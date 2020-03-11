# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_COMMAND = /home/rak/clion-2019.1.4/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/rak/clion-2019.1.4/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/rak/Dropbox/Dev/Vortexy

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/rak/Dropbox/Dev/Vortexy

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/home/rak/clion-2019.1.4/bin/cmake/linux/bin/cmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/home/rak/clion-2019.1.4/bin/cmake/linux/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/rak/Dropbox/Dev/Vortexy/CMakeFiles /home/rak/Dropbox/Dev/Vortexy/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/rak/Dropbox/Dev/Vortexy/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named Vortexy

# Build rule for target.
Vortexy: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 Vortexy
.PHONY : Vortexy

# fast build rule for target.
Vortexy/fast:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/build
.PHONY : Vortexy/fast

src/main.o: src/main.c.o

.PHONY : src/main.o

# target to build an object file
src/main.c.o:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/main.c.o
.PHONY : src/main.c.o

src/main.i: src/main.c.i

.PHONY : src/main.i

# target to preprocess a source file
src/main.c.i:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/main.c.i
.PHONY : src/main.c.i

src/main.s: src/main.c.s

.PHONY : src/main.s

# target to generate assembly for a file
src/main.c.s:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/main.c.s
.PHONY : src/main.c.s

src/phys/fluid.o: src/phys/fluid.c.o

.PHONY : src/phys/fluid.o

# target to build an object file
src/phys/fluid.c.o:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/phys/fluid.c.o
.PHONY : src/phys/fluid.c.o

src/phys/fluid.i: src/phys/fluid.c.i

.PHONY : src/phys/fluid.i

# target to preprocess a source file
src/phys/fluid.c.i:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/phys/fluid.c.i
.PHONY : src/phys/fluid.c.i

src/phys/fluid.s: src/phys/fluid.c.s

.PHONY : src/phys/fluid.s

# target to generate assembly for a file
src/phys/fluid.c.s:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/phys/fluid.c.s
.PHONY : src/phys/fluid.c.s

src/phys/mesh.o: src/phys/mesh.c.o

.PHONY : src/phys/mesh.o

# target to build an object file
src/phys/mesh.c.o:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/phys/mesh.c.o
.PHONY : src/phys/mesh.c.o

src/phys/mesh.i: src/phys/mesh.c.i

.PHONY : src/phys/mesh.i

# target to preprocess a source file
src/phys/mesh.c.i:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/phys/mesh.c.i
.PHONY : src/phys/mesh.c.i

src/phys/mesh.s: src/phys/mesh.c.s

.PHONY : src/phys/mesh.s

# target to generate assembly for a file
src/phys/mesh.c.s:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/phys/mesh.c.s
.PHONY : src/phys/mesh.c.s

src/phys/obj.o: src/phys/obj.c.o

.PHONY : src/phys/obj.o

# target to build an object file
src/phys/obj.c.o:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/phys/obj.c.o
.PHONY : src/phys/obj.c.o

src/phys/obj.i: src/phys/obj.c.i

.PHONY : src/phys/obj.i

# target to preprocess a source file
src/phys/obj.c.i:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/phys/obj.c.i
.PHONY : src/phys/obj.c.i

src/phys/obj.s: src/phys/obj.c.s

.PHONY : src/phys/obj.s

# target to generate assembly for a file
src/phys/obj.c.s:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/phys/obj.c.s
.PHONY : src/phys/obj.c.s

src/phys/solver.o: src/phys/solver.c.o

.PHONY : src/phys/solver.o

# target to build an object file
src/phys/solver.c.o:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/phys/solver.c.o
.PHONY : src/phys/solver.c.o

src/phys/solver.i: src/phys/solver.c.i

.PHONY : src/phys/solver.i

# target to preprocess a source file
src/phys/solver.c.i:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/phys/solver.c.i
.PHONY : src/phys/solver.c.i

src/phys/solver.s: src/phys/solver.c.s

.PHONY : src/phys/solver.s

# target to generate assembly for a file
src/phys/solver.c.s:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/phys/solver.c.s
.PHONY : src/phys/solver.c.s

src/phys/sys.o: src/phys/sys.c.o

.PHONY : src/phys/sys.o

# target to build an object file
src/phys/sys.c.o:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/phys/sys.c.o
.PHONY : src/phys/sys.c.o

src/phys/sys.i: src/phys/sys.c.i

.PHONY : src/phys/sys.i

# target to preprocess a source file
src/phys/sys.c.i:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/phys/sys.c.i
.PHONY : src/phys/sys.c.i

src/phys/sys.s: src/phys/sys.c.s

.PHONY : src/phys/sys.s

# target to generate assembly for a file
src/phys/sys.c.s:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/phys/sys.c.s
.PHONY : src/phys/sys.c.s

src/phys/volume.o: src/phys/volume.c.o

.PHONY : src/phys/volume.o

# target to build an object file
src/phys/volume.c.o:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/phys/volume.c.o
.PHONY : src/phys/volume.c.o

src/phys/volume.i: src/phys/volume.c.i

.PHONY : src/phys/volume.i

# target to preprocess a source file
src/phys/volume.c.i:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/phys/volume.c.i
.PHONY : src/phys/volume.c.i

src/phys/volume.s: src/phys/volume.c.s

.PHONY : src/phys/volume.s

# target to generate assembly for a file
src/phys/volume.c.s:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/phys/volume.c.s
.PHONY : src/phys/volume.c.s

src/render/draw.o: src/render/draw.c.o

.PHONY : src/render/draw.o

# target to build an object file
src/render/draw.c.o:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/render/draw.c.o
.PHONY : src/render/draw.c.o

src/render/draw.i: src/render/draw.c.i

.PHONY : src/render/draw.i

# target to preprocess a source file
src/render/draw.c.i:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/render/draw.c.i
.PHONY : src/render/draw.c.i

src/render/draw.s: src/render/draw.c.s

.PHONY : src/render/draw.s

# target to generate assembly for a file
src/render/draw.c.s:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/render/draw.c.s
.PHONY : src/render/draw.c.s

src/render/render.o: src/render/render.c.o

.PHONY : src/render/render.o

# target to build an object file
src/render/render.c.o:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/render/render.c.o
.PHONY : src/render/render.c.o

src/render/render.i: src/render/render.c.i

.PHONY : src/render/render.i

# target to preprocess a source file
src/render/render.c.i:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/render/render.c.i
.PHONY : src/render/render.c.i

src/render/render.s: src/render/render.c.s

.PHONY : src/render/render.s

# target to generate assembly for a file
src/render/render.c.s:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/render/render.c.s
.PHONY : src/render/render.c.s

src/render/shader.o: src/render/shader.c.o

.PHONY : src/render/shader.o

# target to build an object file
src/render/shader.c.o:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/render/shader.c.o
.PHONY : src/render/shader.c.o

src/render/shader.i: src/render/shader.c.i

.PHONY : src/render/shader.i

# target to preprocess a source file
src/render/shader.c.i:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/render/shader.c.i
.PHONY : src/render/shader.c.i

src/render/shader.s: src/render/shader.c.s

.PHONY : src/render/shader.s

# target to generate assembly for a file
src/render/shader.c.s:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/render/shader.c.s
.PHONY : src/render/shader.c.s

src/render/texture.o: src/render/texture.c.o

.PHONY : src/render/texture.o

# target to build an object file
src/render/texture.c.o:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/render/texture.c.o
.PHONY : src/render/texture.c.o

src/render/texture.i: src/render/texture.c.i

.PHONY : src/render/texture.i

# target to preprocess a source file
src/render/texture.c.i:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/render/texture.c.i
.PHONY : src/render/texture.c.i

src/render/texture.s: src/render/texture.c.s

.PHONY : src/render/texture.s

# target to generate assembly for a file
src/render/texture.c.s:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/render/texture.c.s
.PHONY : src/render/texture.c.s

src/render/window.o: src/render/window.c.o

.PHONY : src/render/window.o

# target to build an object file
src/render/window.c.o:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/render/window.c.o
.PHONY : src/render/window.c.o

src/render/window.i: src/render/window.c.i

.PHONY : src/render/window.i

# target to preprocess a source file
src/render/window.c.i:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/render/window.c.i
.PHONY : src/render/window.c.i

src/render/window.s: src/render/window.c.s

.PHONY : src/render/window.s

# target to generate assembly for a file
src/render/window.c.s:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/render/window.c.s
.PHONY : src/render/window.c.s

src/sim.o: src/sim.c.o

.PHONY : src/sim.o

# target to build an object file
src/sim.c.o:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/sim.c.o
.PHONY : src/sim.c.o

src/sim.i: src/sim.c.i

.PHONY : src/sim.i

# target to preprocess a source file
src/sim.c.i:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/sim.c.i
.PHONY : src/sim.c.i

src/sim.s: src/sim.c.s

.PHONY : src/sim.s

# target to generate assembly for a file
src/sim.c.s:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/sim.c.s
.PHONY : src/sim.c.s

src/util/algebra.o: src/util/algebra.c.o

.PHONY : src/util/algebra.o

# target to build an object file
src/util/algebra.c.o:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/util/algebra.c.o
.PHONY : src/util/algebra.c.o

src/util/algebra.i: src/util/algebra.c.i

.PHONY : src/util/algebra.i

# target to preprocess a source file
src/util/algebra.c.i:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/util/algebra.c.i
.PHONY : src/util/algebra.c.i

src/util/algebra.s: src/util/algebra.c.s

.PHONY : src/util/algebra.s

# target to generate assembly for a file
src/util/algebra.c.s:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/util/algebra.c.s
.PHONY : src/util/algebra.c.s

src/util/mat.o: src/util/mat.c.o

.PHONY : src/util/mat.o

# target to build an object file
src/util/mat.c.o:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/util/mat.c.o
.PHONY : src/util/mat.c.o

src/util/mat.i: src/util/mat.c.i

.PHONY : src/util/mat.i

# target to preprocess a source file
src/util/mat.c.i:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/util/mat.c.i
.PHONY : src/util/mat.c.i

src/util/mat.s: src/util/mat.c.s

.PHONY : src/util/mat.s

# target to generate assembly for a file
src/util/mat.c.s:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/util/mat.c.s
.PHONY : src/util/mat.c.s

src/util/util.o: src/util/util.c.o

.PHONY : src/util/util.o

# target to build an object file
src/util/util.c.o:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/util/util.c.o
.PHONY : src/util/util.c.o

src/util/util.i: src/util/util.c.i

.PHONY : src/util/util.i

# target to preprocess a source file
src/util/util.c.i:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/util/util.c.i
.PHONY : src/util/util.c.i

src/util/util.s: src/util/util.c.s

.PHONY : src/util/util.s

# target to generate assembly for a file
src/util/util.c.s:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/util/util.c.s
.PHONY : src/util/util.c.s

src/util/vec.o: src/util/vec.c.o

.PHONY : src/util/vec.o

# target to build an object file
src/util/vec.c.o:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/util/vec.c.o
.PHONY : src/util/vec.c.o

src/util/vec.i: src/util/vec.c.i

.PHONY : src/util/vec.i

# target to preprocess a source file
src/util/vec.c.i:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/util/vec.c.i
.PHONY : src/util/vec.c.i

src/util/vec.s: src/util/vec.c.s

.PHONY : src/util/vec.s

# target to generate assembly for a file
src/util/vec.c.s:
	$(MAKE) -f CMakeFiles/Vortexy.dir/build.make CMakeFiles/Vortexy.dir/src/util/vec.c.s
.PHONY : src/util/vec.c.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... Vortexy"
	@echo "... edit_cache"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
	@echo "... src/phys/fluid.o"
	@echo "... src/phys/fluid.i"
	@echo "... src/phys/fluid.s"
	@echo "... src/phys/mesh.o"
	@echo "... src/phys/mesh.i"
	@echo "... src/phys/mesh.s"
	@echo "... src/phys/obj.o"
	@echo "... src/phys/obj.i"
	@echo "... src/phys/obj.s"
	@echo "... src/phys/solver.o"
	@echo "... src/phys/solver.i"
	@echo "... src/phys/solver.s"
	@echo "... src/phys/sys.o"
	@echo "... src/phys/sys.i"
	@echo "... src/phys/sys.s"
	@echo "... src/phys/volume.o"
	@echo "... src/phys/volume.i"
	@echo "... src/phys/volume.s"
	@echo "... src/render/draw.o"
	@echo "... src/render/draw.i"
	@echo "... src/render/draw.s"
	@echo "... src/render/render.o"
	@echo "... src/render/render.i"
	@echo "... src/render/render.s"
	@echo "... src/render/shader.o"
	@echo "... src/render/shader.i"
	@echo "... src/render/shader.s"
	@echo "... src/render/texture.o"
	@echo "... src/render/texture.i"
	@echo "... src/render/texture.s"
	@echo "... src/render/window.o"
	@echo "... src/render/window.i"
	@echo "... src/render/window.s"
	@echo "... src/sim.o"
	@echo "... src/sim.i"
	@echo "... src/sim.s"
	@echo "... src/util/algebra.o"
	@echo "... src/util/algebra.i"
	@echo "... src/util/algebra.s"
	@echo "... src/util/mat.o"
	@echo "... src/util/mat.i"
	@echo "... src/util/mat.s"
	@echo "... src/util/util.o"
	@echo "... src/util/util.i"
	@echo "... src/util/util.s"
	@echo "... src/util/vec.o"
	@echo "... src/util/vec.i"
	@echo "... src/util/vec.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

