# Vortexy - Fluid dynamics simulator

Vortexy, CFD code written in C. Uses the finite volume method and SIMPLE algorithm to solve Navier-Stokes equations for incompressible fluids. Includes an OpenGL renderer for visualization. Output can also be analyzed using the included Mathematica notebooks.

Usage:

```
./Vortexy <-s / -r> <sim.cfg>
```
-s = simulate, -r = render

Compilation:

```
cmake .
make
```

Or non-graphical, statically linked (set RENDER_ENABLED to 0 in sim.h)

```
cmake -DRENDER=OFF .
make
```

The script `build.sh` can be used to automatically compile either in graphical on non-graphical mode.

<p align="middle">
  <img src="https://user-images.githubusercontent.com/12766039/109259406-ce395c00-7804-11eb-9f96-ba6f32aa5e1b.png" width="49%" />
  <img src="https://user-images.githubusercontent.com/12766039/109259469-f1640b80-7804-11eb-9deb-fc93af566ffc.png" width="49%" />
</p>

## Dependencies

sim
- glibc, libm

render
- OpenGL
- GLEW
- GLFW

## Technical description

- finite volume method
- SIMPLE algorithm
- Rhie-Chow interpolation with correction terms
- up to second-order upwind convection schemes
- Gauss-Seidel solver
- irregular tetrahedral meshes: boundary .stl -> volume .mesh (e.g. using Tetgen)
- boundary conditions defined in a .cfg file

### Renderer controls

| Key | Action |
| --- | ------ |
| WASD | Move camera XZ |
| R / F | Move camera Y |
| Arrows | Rotate camera XY |
| Mouse drag | Rotate scene Y |
| Scroll | Zoom |
| 1 | Toggle mesh render |
| 2 | Wireframe |
| 3 | Lines |
| 4 | Vectors |
| I | Start simulation |
| O | Stop simulation |
| U | Reset simulation |
