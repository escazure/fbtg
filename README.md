### Terrain generator and visualizer
*Project for university*

Wraps UDF into a glsl file, which gets used for height map generation.
Provides library with multiple utility functions for height map generation.

Implements:
- Terrain: procedurally generated using UDF in compute shaders.
- Shadows: baked into texture with raymarcher in compute shaders.
- Skybox: procedurally generated, with vertical gradient and atmospheric halo glow for sun disk.
- Water: uses warped voronoi noise to fake water caustics and terrain reflection.

*By default uses discrete GPU.*
*Works only on linux.*<br/>

#### Images 
---
<div align="left">
    <img src="assets/generated_terrain_example1.png" width="350"/>
</div>

<div align="left">
    <img src="assets/generated_terrain_example2.png" width="350"/>
</div>

<div align="left">
    <img src="assets/generated_terrain_example3.png" width="350"/>
</div>

<div align="left">
    <img src="assets/4k_plane.png" width="350"/>
</div>

<div align="left">
    <img src="assets/4k_plane_with_water.png" width="350"/>
</div>

<div align="left">
    <img src="assets/4k_plane_altitude_map.png" width="350"/>
</div>

<div align="left">
    <img src="assets/4k_plane_greyscale_map.png" width="350"/>
</div>

#### Dependencies
---
- GLFW for window management
- GL3W for OpenGL function loading
- OpenGL for rendering
- GLM for math
- ImGui for GUI
- LodePNG for png import/export

#### Run
---
```./build/tgv```

Arguments:
- `--log`: Logs to console various information during runtime.
- `--debug`: Enables OpenGL debug callback.
- `--integrated`: Uses integrated GPU instead of discrete.

#### Build
---
```cmake --build build```<br/>
```./build/tgv```
