#pragma once
#include <filesystem>
#include <string>

#include "ResourceType.hpp"

class Texture {
public:
    using ResourceIDType = TextureID;
    explicit Texture(const std::filesystem::path& path);
    ~Texture();

    size_t GetMemoryUsage() const noexcept {
        return m_Width * m_Height * m_BPP;  // RGBA8888
    }

    // 禁用拷贝
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    // 移动语义
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;
    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }

private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BPP;  // Bits Per Pixel
};
