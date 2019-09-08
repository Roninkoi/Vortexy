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
- two boundary conditions: open (do nothing) / closed (wall)

### Controls

```
W ------- cam forward
A ------- cam left
S ------- cam backward
D ------- cam right

R ------- cam up
F ------- cam down

UP ------ look up
DOWN ---- look down
LEFT ---- look left
RIGHT --- look right

MOUSE --- rotate
SCROLL -- zoom
```
