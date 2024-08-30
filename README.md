A simple demo for Conway's Game of Life written in C++ with OpenGL textures.

**Controls:**
- [Space] : Pause or resume simulation
- c : Toggle cell state at cursor

**Requires:**
- CMake
- GLFW

Download glfw*
```
E.g. using arch:
$ sudo pacman -Sy glfw
```

Compile and run
```
$ cmake .
$ make
$ ./gol
```

*If you are on windows, you can download GLFW source code and link to the /include/ directory using the commented out line in the CMakeLists.txt file 
