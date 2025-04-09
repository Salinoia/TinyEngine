# TinyEngine

![OpenGL](https://img.shields.io/badge/OpenGL-3.3%2B-blue)
![License](https://img.shields.io/badge/License-MIT-green)

A lightweight OpenGL-based game engine for educational purposes and graphics application development.

## Features

- Modern OpenGL 3.3 Core Profile rendering
- ImGui integration for debug UI
- 2D texture rendering support
- Test framework with scene switching
- Cross-platform window management (GLFW)
- Error handling with OpenGL call validation
- Shader and texture resource management

## Dependencies

### Core Libraries

- OpenGL 3.3+
- GLFW 3.x
- GLEW 2.x
- Eigen3
- spdlog
- Vulkan SDK (for shader compilation)
- stb_image (included)
- glm (included)
- Dear ImGui (included)

### Recommended Installation via vcpkg

```bash
./vcpkg install glfw3 glew eigen3 spdlog
```

## Build Instructions

### Prerequisites

- CMake 3.28+
- C++17 compatible compiler
- Vulkan SDK (optional for shader tools)

### Build Steps

```bash
git clone https://github.com/Salinoia/TinyEngine
cd TinyEngine
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build .
```

## Project Structure

```
TinyEngine/
├── include/           # Engine headers
│   ├── vendor/        # Third-party libraries
├── src/
│   ├── Core/          # Core implementation
│   ├── Platform/      # OpenGL platform code
│   ├── Tests/         # Test scenes
│   └── Utils/         # Utility classes
├── res/               # Resource files
│   ├── shaders/       # GLSL shaders
│   └── textures/      # Texture assets
└── CMakeLists.txt     # Build configuration
```

## Usage Example

```cpp
// Initialize engine components
GLFWwindow* window = glfwCreateWindow(...);
OpenGLRenderer renderer;

// Create test scene
auto testScene = std::make_unique<test::TestTexture2D>(
    PresetIDs::Shaders::Basic2D,
    PresetIDs::Textures::TestPNG,
    "res/shaders/Basic.vert",
    "res/shaders/Basic.frag",
    "res/textures/Google.png"
);

// Main loop
while (!glfwWindowShouldClose(window)) {
    renderer.Clear();
    testScene->OnUpdate(deltaTime);
    testScene->OnRender();
    
    // ImGui interface
    ImGui::Begin("Debug Panel");
    if (ImGui::Button("Reset Scene")) {
        testScene->Reset();
    }
    ImGui::End();
}
```

## Key Bindings

- `ESC`: Close window
- `<-`: Return to main menu

## Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository
2. Create a feature branch
3. Submit a pull request
4. Ensure all tests pass

## License

[MIT License](LICENSE)

```
This README includes:
1. Badges for quick project status
2. Clear feature highlights
3. Detailed dependency installation instructions
4. Step-by-step build guide
5. Visual project structure explanation
6. Practical code example
7. Contribution guidelines
8. License information

The structure emphasizes practical usage while maintaining technical accuracy for OpenGL developers.
```

