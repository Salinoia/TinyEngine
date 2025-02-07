#include "VertexArray.h"

#include "Renderer.h"
VertexArray::VertexArray() {
    // Create and bind Vertex Array Object (VAO)
    GLCall(glGenVertexArrays(1, &m_RendererID));  // Generate a VAO
}

VertexArray::~VertexArray() {
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
    this->Bind();
    vb.Bind();
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    for (int i = 0; i < elements.size(); ++i) {
        const auto& element = elements[i];
        // Configure Vertex Attribute Pointer
        GLCall(glEnableVertexAttribArray(i));  // Enable vertex attribute index 0

        /*
         * Set vertex attribute pointer to describe the layout of vertex data
         * void glVertexAttribPointer(
         *      GLuint index,         // Vertex attribute index (matches layout(location = 0) in
         *      vertex shader) GLint size,           // Number of components per vertex (e.g., 2 for
         *      vec2, 3 for vec3) GLenum type,          // Data type (e.g., GL_FLOAT for floating point
         *      numbers) GLboolean normalized, // Whether to normalize (GL_FALSE means no normalization)
         *      GLsizei stride,       // Stride (byte offset between consecutive vertices)
         *      const void *pointer   // Starting offset (byte offset from the beginning of the vbo)
         * )
         */

        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized,
            layout.GetStride(),reinterpret_cast<const void*>(static_cast<uintptr_t>(offset))));  // Configure vertex attribute pointer
        offset += element.count * VertexBufferElement::SizeofGLTypes(element.type);
    }
}

void VertexArray::Bind() const{
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const{
    GLCall(glBindVertexArray(0));
}