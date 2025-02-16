#pragma once

#include <spdlog/spdlog.h>

#include <chrono>
#include <filesystem>
#include <functional>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <unordered_map>

#include "ResourceType.hpp"
#include "Shader.h"
#include "Texture.h"

/**
 * @brief 资源管理器（静态工具类）
 *
 * 特性：
 * 1. 自动引用计数管理（weak_ptr + shared_ptr 组合）
 * 2. 类型安全资源标识符
 * 3. 读写锁优化并发性能
 * 4. 异常安全设计
 * 5. 内存使用统计
 */
class ResourceManager {
public:
    // 删除构造函数和赋值操作
    ResourceManager() = delete;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    /// @brief 加载或获取已缓存的着色器
    /// @return 成功返回shared_ptr，失败返回错误信息
    static std::pair<std::shared_ptr<Shader>, std::error_code> LoadShader(
        ShaderID id, const std::filesystem::path& vsPath, const std::filesystem::path& fsPath);

    /// @brief 加载或获取已缓存的纹理
    static std::pair<std::shared_ptr<Texture>, std::error_code> LoadTexture(
        TextureID id, const std::filesystem::path& path);

    /// @brief 获取已加载的着色器（不触发加载）
    static std::shared_ptr<Shader> GetShader(ShaderID id) noexcept;

    /// @brief 获取已加载的纹理（不触发加载）
    static std::shared_ptr<Texture> GetTexture(TextureID id) noexcept;

    /// @brief 释放指定资源
    static void Unload(ShaderID id) noexcept;
    static void Unload(TextureID id) noexcept;

    /// @brief 清空所有缓存资源
    static void Clear() noexcept;

    /// @brief 获取资源内存使用统计（单位：字节）
    static size_t GetShaderMemoryUsage() noexcept;
    static size_t GetTextureMemoryUsage() noexcept;

    /// @brief 启用文件监控自动重载
    static void EnableHotReload(
        std::chrono::milliseconds checkInterval = std::chrono::milliseconds(1000));

private:
    // 通用资源缓存模板
    template <typename Res>
    class ResourceCache {
    public:
        using IDType = typename Res::ResourceIDType;

        ResourceCache() = default;

        ResourceCache(const ResourceCache&) = delete;
        ResourceCache& operator=(const ResourceCache&) = delete;
        ResourceCache(ResourceCache&&) = default;
        ResourceCache& operator=(ResourceCache&&) = default;

        void Clear() noexcept {
            std::unique_lock lock(m_mutex);
            m_cache.clear();
        }

        // 修改加载器返回类型
        std::pair<std::shared_ptr<Res>, std::error_code> Load(
            const IDType& id,
            std::function<std::pair<std::shared_ptr<Res>, std::error_code>()> loader) {
            // 双重检查锁定模式
            {
                std::shared_lock readLock(m_mutex);
                if (auto it = m_cache.find(id); it != m_cache.end()) {
                    if (auto sp = it->second.lock()) {
                        return {sp, {}};
                    }
                }
            }

            // 加载前临时解锁
            auto [resource, ec] = loader();
            if (ec)
                return {nullptr, ec};

            {
                std::unique_lock writeLock(m_mutex);
                auto& weakRes = m_cache[id];  // 直接更新现有条目

                // 确保旧资源完全释放
                if (auto oldRes = weakRes.lock()) {
                    spdlog::warn("Resource ID:{} already loaded, replacing...", id.value);
                }

                weakRes = resource;
                return {resource, {}};
            }
        }

        std::shared_ptr<Res> Get(const IDType& id) const noexcept {
            std::shared_lock lock(m_mutex);
            if (auto it = m_cache.find(id); it != m_cache.end()) {
                return it->second.lock();
            }
            return nullptr;
        }

        void Unload(const IDType& id) noexcept {
            std::unique_lock lock(m_mutex);
            if (auto it = m_cache.find(id); it != m_cache.end()) {
                // 只有当没有外部引用时才会实际删除
                if (it->second.expired()) {
                    m_cache.erase(it);
                }
            }
        }

        size_t MemoryUsage() const noexcept {
            std::shared_lock lock(m_mutex);
            size_t total = 0;
            for (const auto& [id, weakRes] : m_cache) {
                if (auto res = weakRes.lock()) {
                    total += res->GetMemoryUsage();
                }
            }
            return total;
        }

    private:
        mutable std::shared_mutex m_mutex;
        std::unordered_map<IDType, std::weak_ptr<Res>, typename IDType::Hasher> m_cache;
    };

    // 静态成员初始化
    inline static ResourceCache<Shader> s_shaderCache;
    inline static ResourceCache<Texture> s_textureCache;
};
