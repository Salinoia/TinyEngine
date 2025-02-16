#include "OpenGLIndexBuffer.h"

#include "OpenGLRenderer.h"  // 包含GLCall宏

namespace OpenGLGraphics {

IndexBufferImpl::IndexBufferImpl(const unsigned int* data, unsigned int count) : m_Count(count) {
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GLCall(
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBufferImpl::~IndexBufferImpl() {
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBufferImpl::Bind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBufferImpl::Unbind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

}  // namespace OpenGLGraphics
