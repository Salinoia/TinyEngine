cmake_minimum_required(VERSION 3.28)
project(GameEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines")
set(CMAKE_CXX_SCAN_FOR_MODULES_PARSER "clang-scan-deps-18")
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")

find_package(Vulkan REQUIRED COMPONENTS glslc)
find_package(Eigen3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/vendor/stb_image)
include_directories(${CMAKE_SOURCE_DIR}/include/vendor/glm)
include_directories(${CMAKE_SOURCE_DIR}/include/vendor/glm/gtc)
include_directories(${CMAKE_SOURCE_DIR}/include/vendor/imgui)
include_directories(${CMAKE_SOURCE_DIR}/Tests)

file(GLOB SOURCES 
    "${CMAKE_SOURCE_DIR}/include/vendor/stb_image/*.cpp"
    "${CMAKE_SOURCE_DIR}/include/vendor/imgui/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.cpp" 
    "${CMAKE_SOURCE_DIR}/Tests/*.cpp" 
    )

add_executable(Engine ${SOURCES})
target_link_libraries(Engine PRIVATE 
    Vulkan::Vulkan 
    glfw
    Eigen3::Eigen
    OpenGL
    GLEW::GLEW
    imgui
)