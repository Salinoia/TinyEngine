#include "OpenGLVertexBuffer.h"

#include "OpenGLRenderer.h"  // 包含GLCall宏

namespace OpenGLGraphics {

VertexBufferImpl::VertexBufferImpl(const void* data, size_t size, BufferUsage usage) :
    m_Size(size), m_Usage(usage) {
    GLCall(glGenBuffers(1, &m_RendererID));
    Bind();
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, UsageToGL(usage)));
}

VertexBufferImpl::~VertexBufferImpl() {
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBufferImpl::Bind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBufferImpl::Unbind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBufferImpl::UpdateData(const void* data, size_t size, size_t offset) {
    if (offset + size > m_Size) {
        throw std::out_of_range("Vertex buffer update exceeds buffer size");
    }
    Bind();
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
}

GLenum VertexBufferImpl::UsageToGL(BufferUsage usage) noexcept {
    switch (usage) {
    case BufferUsage::StaticDraw:
        return GL_STATIC_DRAW;
    case BufferUsage::DynamicDraw:
        return GL_DYNAMIC_DRAW;
    case BufferUsage::StreamDraw:
        return GL_STREAM_DRAW;
    default:
        return GL_STATIC_DRAW;
    }
}

// 移动语义实现
VertexBufferImpl::VertexBufferImpl(VertexBufferImpl&& other) noexcept :
    m_RendererID(other.m_RendererID), m_Size(other.m_Size), m_Usage(other.m_Usage) {
    other.m_RendererID = 0;  // 防止原对象删除资源
    other.m_Size = 0;
}

VertexBufferImpl& VertexBufferImpl::operator=(VertexBufferImpl&& other) noexcept {
    if (this != &other) {
        GLCall(glDeleteBuffers(1, &m_RendererID));

        m_RendererID = other.m_RendererID;
        m_Size = other.m_Size;
        m_Usage = other.m_Usage;

        other.m_RendererID = 0;
        other.m_Size = 0;
    }
    return *this;
}

}  // namespace OpenGLGraphics
