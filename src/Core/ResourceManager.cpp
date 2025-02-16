#include "ResourceManager.hpp"

#include <system_error>

// 显式实例化模板类
template class ResourceManager::ResourceCache<Shader>;
template class ResourceManager::ResourceCache<Texture>;

// Shader相关操作
std::pair<std::shared_ptr<Shader>, std::error_code>
ResourceManager::LoadShader(
    ShaderID id, const std::filesystem::path& vsPath, const std::filesystem::path& fsPath) {
    return s_shaderCache.Load(id, [&]() -> std::pair<std::shared_ptr<Shader>, std::error_code> {
        try {
            auto shader = std::make_shared<Shader>(vsPath, fsPath);
            spdlog::info("Loaded shader: {} (ID: {})", vsPath.filename().string(), id.value);
            return {shader, {}};
        } catch (const std::exception& e) {
            spdlog::error("Failed to load shader {}: {}", id.value, e.what());
            return {nullptr, std::make_error_code(std::errc::io_error)};
        }
    });
}

std::shared_ptr<Shader>
ResourceManager::GetShader(ShaderID id) noexcept {
    return s_shaderCache.Get(id);
}

void
ResourceManager::Unload(ShaderID id) noexcept {
    s_shaderCache.Unload(id);
}

// Texture相关操作
std::pair<std::shared_ptr<Texture>, std::error_code>
ResourceManager::LoadTexture(TextureID id, const std::filesystem::path& path) {
    return s_textureCache.Load(id, [&]() -> std::pair<std::shared_ptr<Texture>, std::error_code> {
        try {
            auto texture = std::make_shared<Texture>(path);
            spdlog::info("Loaded texture: {} (ID: {})", path.filename().string(), id.value);
            return {texture, {}};
        } catch (const std::exception& e) {
            spdlog::error("Failed to load texture {}: {}", id.value, e.what());
            return {nullptr, std::make_error_code(std::errc::io_error)};
        }
    });
}

std::shared_ptr<Texture>
ResourceManager::GetTexture(TextureID id) noexcept {
    return s_textureCache.Get(id);
}

void
ResourceManager::Unload(TextureID id) noexcept {
    s_textureCache.Unload(id);
}

// 资源管理
void
ResourceManager::Clear() noexcept {
    spdlog::warn("Clearing all cached resources");
    s_shaderCache.Clear();
    s_textureCache.Clear();
}

// 内存统计
size_t
ResourceManager::GetShaderMemoryUsage() noexcept {
    return s_shaderCache.MemoryUsage();
}

size_t
ResourceManager::GetTextureMemoryUsage() noexcept {
    return s_textureCache.MemoryUsage();
}

// 热重载实现（基础版）
void
ResourceManager::EnableHotReload(std::chrono::milliseconds checkInterval) {
    // 需要文件监控库（如efsw）的完整实现
    spdlog::warn("Hot reload is not fully implemented yet. checkInterval: {}ms", checkInterval.count());
}
