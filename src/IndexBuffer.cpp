#include "IndexBuffer.h"

#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : m_Count(count) {
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));
    GLCall(glGenBuffers(1, &m_RendererID));  // Generate a VBO
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                        m_RendererID));  // Bind VBO to the GL_ARRAY_BUFFER target
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data,
                        GL_STATIC_DRAW));  // Upload vertex data to GPU
}

IndexBuffer::~IndexBuffer() {
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}