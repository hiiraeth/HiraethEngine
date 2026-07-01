# HiraethEngine

A real-time 3D rendering engine written in modern C++ with OpenGL. It renders
lit, textured scenes with loaded 3D models and provides a live ImGui control
panel for editing the scene at runtime — toggling and positioning lights,
placing models, and adjusting materials without recompiling.

Built as a study of real-time graphics fundamentals (following established
OpenGL techniques) and extended with scene-editing, material, and model-
management systems.

## Features

- **Blinn-Phong lighting** with multiple light types (directional, point, spot),
  each independently configurable at runtime.
- **Texture mapping** with separate diffuse and specular maps, plus fallback
  material properties when maps are absent.
- **Model loading via Assimp** — imports external 3D model formats, extracts
  meshes, materials, and textures, and caches loaded textures to avoid
  redundant GPU uploads.
- **Cubemap skybox** rendered with a depth trick so it always sits behind scene
  geometry.
- **A built-in material library** of physically-inspired presets (metals, gems,
  plastics, rubbers).
- **Live scene editor (ImGui)** — enable/disable lights, move models and lights,
  and swap materials while the engine runs.
- **Free-fly camera** with mouse-look, WASD/QE movement, and scroll-to-adjust
  movement speed.
- **Delta-time render loop** with an FPS counter in the window title.

## Architecture

The engine is organized into modular subsystems, each with a focused
responsibility:

| Component        | Responsibility |
|------------------|----------------|
| `View`           | Window creation, GLFW/GLAD initialization, input processing, callbacks |
| `Shaders`        | Loading, compiling, linking GLSL programs; uniform setters |
| `Shapes`         | Primitive mesh geometry (cube, plane) and their vertex buffers |
| `Mesh`           | A single renderable mesh — VAO/VBO/EBO setup and draw |
| `Model`          | Assimp-loaded models composed of multiple meshes |
| `ModelLibrary`   | Tag-based caching and lifetime management of loaded models |
| `Texture`        | 2D texture and cubemap loading (via stb_image) |
| `Material` / `MaterialLibrary` | Material presets and tag-based lookup |
| `Scene`          | Ties it together — prepares and renders the scene, manages lights and draw calls |
| `Menu`           | ImGui control panel for runtime scene editing |

The render loop lives in `main.cpp`: it computes delta time, clears the frame,
renders the scene, processes input, draws the ImGui menu, and swaps buffers.

## Rendering pipeline (per frame)

1. Update the camera view matrix.
2. Bind the lighting shader and upload light uniforms.
3. Draw the floor plane, then any visible loaded models, applying their
   materials and textures.
4. Draw a small cube at each enabled light's position as a visual marker.
5. Draw the skybox last, using `GL_LEQUAL` depth testing so it renders only
   where no geometry already occupies the depth buffer.

## Building

### Dependencies

- A C++ compiler with C++17 support
- [GLFW](https://www.glfw.org/) — windowing and input
- [GLAD](https://glad.dav1d.de/) — OpenGL function loading
- [GLM](https://github.com/g-truc/glm) — math (vectors, matrices, transforms)
- [Assimp](https://github.com/assimp/assimp) — model import
- [Dear ImGui](https://github.com/ocornut/imgui) — runtime UI
- [stb_image](https://github.com/nothings/stb) — image loading (header-only)

### OpenGL version

The engine requests an OpenGL core profile — 4.1 on macOS (the platform's
maximum) and 3.3 elsewhere — handled automatically at initialization.

### Compiling

Configure and build with your preferred toolchain (CMake project). Ensure the
dependencies above are available to the build, then compile and run the
resulting executable from a location where the relative `../resources/` paths
(shaders, textures, models) resolve.

## Controls

| Input            | Action |
|------------------|--------|
| `W` / `S`        | Move forward / backward |
| `A` / `D`        | Strafe left / right |
| `Q` / `E`        | Move up / down |
| Mouse            | Look around (when cursor is captured) |
| Scroll           | Adjust movement speed |
| `R` / `F`        | Increase / decrease mouse sensitivity |
| `Tab`            | Toggle cursor capture (to interact with the ImGui panel) |
| `Esc`            | Quit |

## Attribution

The core rendering techniques follow the fundamentals taught by
[LearnOpenGL](https://learnopengl.com/) and a graphics programming course.
The material presets are standard OpenGL reference values. The scene-editing
UI, material/model library systems, and the overall engine structure were built
on top of those fundamentals.

## Possible improvements

- Migrate raw owning pointers to `std::unique_ptr` for RAII-based resource
  management (removing manual cleanup entirely).
- Fix and consolidate the texture-loading paths into a single correct routine.
- Replace the loose global cubemap handle with scene-owned state.
- Implement the stubbed sphere primitive.
- Add normal mapping and shadow mapping.
