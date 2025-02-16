#pragma once
#include <GL/glew.h>

#include "VertexArray.h"

namespace OpenGLGraphics {
class VertexArrayImpl final : public IVertexArray {
public:
    VertexArrayImpl();
    ~VertexArrayImpl() override;

    void AddBuffer(const IVertexBuffer& vb, const IVertexLayout& layout) override;
    void Bind() const override;
    void Unbind() const override;
    void Reset();

private:
    mutable GLuint m_VAOID;
    GLuint m_CurrentVBO = 0;  // 跟踪当前绑定的VBO
};
}  // namespace OpenGLGraphics