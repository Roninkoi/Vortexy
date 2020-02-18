# Vortexy - Fluid dynamics simulator

Vortexy, CFD software written in C.

Usage:

```
./Vortexy <sim.cfg>
```

Compilation:

```
cmake .
make
```

## Dependencies

sim
- glibc, (OpenCL)

render
- OpenGL
- GLEW
- glfw

## Technical description

- finite volume method
- SIMPLE algorithm
- irregular triangular meshes
- mesh defined in a file (.mesh/.obj)
- boundary conditions defined in a file

### Controls

| Key | Action |
| --- | ------ |
| WASD | Move camera XZ |
| R / F | Move camera Y |
| Arrows | Rotate camera XY |
| Mouse drag | Rotate scene Y |
| Scroll | Zoom |
