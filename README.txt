Vortexy - Fluid dynamics simulator
==================================

Vortexy "Vorteksi", GPU CFD software written in C using OpenCL.

Usage: ./Vortexy <sim.cfg>

Compilation:

cmake .
make

Dependencies
------------

sim
- OpenCL

render
- OpenGL
- GLEW
- glfw

Technical description
---------------------

- finite volume method
- irregular triangular meshes
- mesh defined in a file (.obj) with a texture (.ppm)
- initial velocity field defined by a texture (sampled at vertices and avgd)
- two boundary conditions: open (do nothing) / closed (wall)
  defined by blue value of texture (0 = open, 1 = closed)

Texture key:

Initial velocity in r-channel, constant flow in g, boundary condition in b

negative (against face normal) [0, 0.5[
no flow 0.5
positive (along face normal) ]0.5, 1.0]

v = 2 * (r - 0.5) * vscale

(initial, constant, boundary)

Code style:

- namespaces are denoted with a prefixed letter and underscore e.g. s_tick(), p_tick(), ...

s = simulator
p = physics
r = renderer

- constructor names are the type e.g. p_vec4()
- initializers are called "Init" e.g. p_vec4Init()
- destructors are called "Destroy" e.g. p_fieldDestroy()
- constructors / initializers should be paired with a destructor if one exists


