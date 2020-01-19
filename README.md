# Vortexy - Fluid dynamics simulator

Vortexy, GPU CFD software written in C using OpenCL.

Usage: ./Vortexy <sim.cfg>

Compilation:

cmake .

make

## Dependencies

sim
- OpenCL

render
- OpenGL
- GLEW
- glfw

## Technical description

- finite volume method
- irregular triangular meshes
- mesh defined in a file (.obj)
- initial and constant velocity field defined
- boundary conditions: inlet / outlet / wall

### Controls

| Key | Action |
| --- | ------ |
| WASD | Move camera XZ |
| R / F | Move camera Y |
| Arrows | Rotate camera XY |
| Mouse drag | Rotate scene Y |
| Scroll | Zoom |

