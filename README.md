# Vortexy - Fluid dynamics simulator

Vortexy, CFD code written in C.

Usage:

```
./Vortexy -s <sim.cfg>
```
-s = simulate, -r = render

Compilation:

```
./build.sh
```

Or non-graphical (set RENDER_ENABLED to 0 in sim.h)

```
./buildsim.sh
```

<p float="left">
  <img src="https://user-images.githubusercontent.com/12766039/102574397-55799d80-40f9-11eb-9c7d-c690e24ee513.png" width="100" />
  <img src="https://user-images.githubusercontent.com/12766039/84868790-e35ec800-b085-11ea-8db5-0219b06b81c2.png" width="100" />
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
- irregular tetrahedral meshes
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
| 1 | Toggle mesh render |
| 2 | Wireframe |
| 3 | Lines |
| 4 | Vectors |
| I | Start simulation |
| O | Stop simulation |
| U | Reset simulation |
