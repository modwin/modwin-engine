# Modwin Engine

Modwin Engine is a learning-focused 2D game engine and game project written in C++17. The project is being built from the ground up to explore engine architecture, real-time rendering, entity management, physics, resource loading, and editor tooling.

The long-term goal is to create a playable 2D game alongside the engine that powers it. A central part of that goal is an ImGui-based level editor that can use the project's existing tilesets to edit levels while the game is running.

> [!IMPORTANT]
> This project is under active development. It is currently a working prototype, not a finished or reusable general-purpose engine.

## Current state

The application currently opens an SDL3 window, loads a TMX tile map and game textures, renders a controllable animated player, and runs ImGui alongside the game.

Implemented systems include:

- SDL3 window creation, rendering, events, keyboard input, and vertical synchronization
- SDL3_image texture loading
- A game loop with delta-time calculation
- Entity and player abstractions
- Basic transform, rigid-body, force, velocity, and friction code
- Animated sprite rendering
- TMX tile-map and tileset parsing with TinyXML2
- Layered tile-map rendering
- Runtime logging with spdlog
- An ImGui context using the SDL3 and SDL renderer backends
- ImGui docking and the built-in demo window
- Initial RAII-based ownership for entity components

The following editor features are planned but are not implemented yet:

- A tileset palette displaying the textures available to the game
- Selecting, painting, replacing, and erasing tiles
- Editing multiple map layers
- Saving edited maps and reloading them without restarting the application
- An editor camera, grid overlay, and world-to-screen coordinate conversion
- Undo and redo
- Object and entity placement
- Separating editor mode from normal play mode

## Project goals

This repository has two connected goals:

1. Build a small 2D engine with enough functionality to support a complete game.
2. Use the engine as a practical way to learn modern C++ and game-engine architecture.

The project intentionally favors understanding the underlying systems over hiding them behind a large framework. Areas of study include ownership and object lifetime, RAII, smart pointers, const correctness, inheritance and composition, data-oriented level representation, game-loop design, resource management, and separation of engine, game, and editor responsibilities.

## Technology

| Component | Purpose |
| --- | --- |
| C++17 | Engine and game implementation |
| CMake | Project configuration and builds |
| SDL3 | Windowing, input, events, and 2D rendering |
| SDL3_image | PNG and image loading |
| Dear ImGui | Runtime editor interface |
| TinyXML2 | TMX map parsing |
| spdlog | Runtime logging |
| Tiled / TMX | Level and tileset data |

Dear ImGui source and its SDL3 renderer backends are currently included under `external/imgui`.

## Project structure

```text
modwin-engine/
|-- include/                 Public headers
|   |-- cfg/                 Input configuration
|   |-- character/           Character and player types
|   |-- component/           Animation and component types
|   |-- core/                Engine entry point and core interfaces
|   |-- entity/              Entity model and entity manager
|   |-- graphics/            Textures, maps, and tile layers
|   |-- input/               SDL input handling
|   |-- physics/             Vectors, transforms, and rigid bodies
|   `-- time/                Frame timing
|-- src/                     Implementations
|-- resources/
|   |-- maps/                TMX levels
|   |-- sprites/             Character sprites
|   `-- tiles/               Tileset textures
|-- external/                Vendored third-party source, including Dear ImGui
`-- CMakeLists.txt           Build configuration
```

The main loop lives in `src/core/Engine.cpp`. In simplified form, each frame performs the following work:

```text
process SDL events
        |
        v
start a new ImGui frame
        |
        v
update the map and entities
        |
        v
render the map, entities, and ImGui
        |
        v
calculate timing for the next frame
```

## Building

### Current development environment

The project is currently developed and tested on Windows with CLion and MinGW. It requires:

- CMake 3.28 or newer
- A C++17 compiler
- SDL3
- SDL3_image
- TinyXML2
- spdlog

The current `CMakeLists.txt` contains absolute Windows paths for several dependencies, including SDL3, SDL3_image, spdlog, and TinyXML2. You must either install those dependencies at the same paths or update the paths for your own machine before configuring the project.

The TinyXML2 source directory is also currently expected at:

```text
C:/TINYXML2_SRC
```

### CLion

1. Open the repository root in CLion.
2. Select a MinGW toolchain with C++17 support.
3. Update the dependency paths in `CMakeLists.txt` if necessary.
4. Reload the CMake project.
5. Build and run the `modwin_game` target.

### Command line

After the dependency paths have been configured, a typical CMake build is:

```powershell
cmake -S . -B cmake-build-debug
cmake --build cmake-build-debug --target modwin_game -j 4
```

Generated executables and libraries are written beneath the selected CMake build directory. For a debug build, the game executable is placed in `cmake-build-debug/bin/`.

## Controls

The default movement controls are:

| Action | Key |
| --- | --- |
| Move up | W |
| Move left | A |
| Move down | S |
| Move right | D |
| Attack | Space |

The attack key is present in the input configuration, but attack behavior is not implemented yet.

## Development roadmap

The next major milestone is a minimal live tile editor:

1. Make map and tile-layer data safely editable at runtime.
2. Remove hard-coded resource and map paths.
3. Add ImGui panels for editor state, layers, and the tile palette.
4. Convert mouse coordinates into map tile coordinates.
5. Paint and erase tiles in the active layer.
6. Save the modified map and verify a save/reload round trip.
7. Add play/edit modes so editor input does not move the player.

After the editor foundation is working, likely engine milestones include collision detection, camera support, entity placement, scene management, and a more complete gameplay loop.

## Known limitations

- Dependency and resource paths are not portable yet.
- Map loading currently targets one development map.
- ImGui displays its demo window but does not yet expose custom editor tools.
- Physics is an early prototype and does not provide collision detection or platforming behavior.
- Engine, game, and editor responsibilities still need clearer boundaries.
- Automated tests and continuous integration have not been added yet.

## Learning notes

This is a first game-engine project, so refactoring is an expected part of development. Current C++ priorities are:

- Prefer automatic storage and `std::unique_ptr` over owning raw pointers.
- Make ownership explicit and avoid manual `new`/`delete` pairs.
- Use const-correct interfaces for read-only operations.
- Keep renderable map data separate from map-file parsing.
- Give systems focused responsibilities instead of placing all behavior in the main engine class.
- Keep changes small and buildable so architectural improvements can be verified incrementally.

## Status

Modwin Engine is a personal educational project in active development. The API, file format handling, build process, and architecture may change substantially as the engine and game evolve.
