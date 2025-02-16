#pragma once
#include <cstdint>
#include <functional>
#include <type_traits>

// 强类型资源标识符模板
template <typename Tag, typename T = uint32_t>
struct ResourceID {
    T value;

    explicit constexpr ResourceID(T val = 0) noexcept : value(val) {}

    constexpr bool operator==(const ResourceID& other) const noexcept {
        return value == other.value;
    }
    constexpr bool operator!=(const ResourceID& other) const noexcept { return !(*this == other); }

    struct Hasher {
        size_t operator()(const ResourceID& id) const noexcept { return std::hash<T>{}(id.value); }
    };
};

// 资源类型标签定义
struct ShaderTag {};
struct TextureTag {};
struct MaterialTag {};
struct MeshTag {};

// 具体资源ID类型
using ShaderID = ResourceID<ShaderTag>;
using TextureID = ResourceID<TextureTag>;
using MaterialID = ResourceID<MaterialTag>;
using MeshID = ResourceID<MeshTag>;

namespace PresetIDs {
// 预定义着色器ID (范围 1000-1999)
namespace Shaders {
constexpr ShaderID Basic2D{1001};
constexpr ShaderID Basic3D{1002};
constexpr ShaderID PBR{1003};
}  // namespace Shaders

// 预定义纹理ID (范围 2000-2999)
namespace Textures {
constexpr TextureID TestPNG{2001};
constexpr TextureID CrateJPG{2002};
constexpr TextureID MetalPlate{2003};
}  // namespace Textures

// 预定义材质ID (范围 3000-3999)
namespace Materials {
constexpr MaterialID DefaultOpaque{3001};
constexpr MaterialID TransparentGlass{3002};
}  // namespace Materials

// 预定义网格ID (范围 4000-4999)
namespace Meshes {
constexpr MeshID Cube{4001};
constexpr MeshID Sphere{4002};
}  // namespace Meshes
}  // namespace PresetIDs