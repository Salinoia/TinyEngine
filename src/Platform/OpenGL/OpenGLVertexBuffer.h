#pragma once
#include <GL/glew.h>

#include "OpenGLGraphics.h"
#include "VertexBuffer.h"

namespace OpenGLGraphics {

class VertexBufferImpl final : public IVertexBuffer {
private:
    GLuint m_RendererID;
    size_t m_Size;        // 字节大小
    BufferUsage m_Usage;  // 使用模式

public:
    explicit VertexBufferImpl(const void* data, size_t size, BufferUsage usage);
    ~VertexBufferImpl() override;

    // 接口实现
    void Bind() const override;
    void Unbind() const override;
    void UpdateData(const void* data, size_t size, size_t offset = 0);

    // 访问器
    inline size_t GetSize() const noexcept { return m_Size; }
    inline GLuint GetID() const noexcept { return m_RendererID; }
    inline BufferUsage GetUsage() const noexcept { return m_Usage; }

    // 移动语义支持
    VertexBufferImpl(VertexBufferImpl&& other) noexcept;
    VertexBufferImpl& operator=(VertexBufferImpl&& other) noexcept;

private:
    // 转换使用模式到OpenGL常量
    static GLenum UsageToGL(BufferUsage usage) noexcept;
};

}  // namespace OpenGLGraphics
