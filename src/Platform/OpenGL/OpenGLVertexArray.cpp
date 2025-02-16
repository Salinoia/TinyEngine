#include "OpenGLVertexArray.h"

#include "OpenGLRenderer.h"
#include "OpenGLVertexBuffer.h"
#include "OpenGLVertexLayout.h"

namespace OpenGLGraphics {
VertexArrayImpl::VertexArrayImpl() {
    GLCall(glGenVertexArrays(1, &m_VAOID));
}

VertexArrayImpl::~VertexArrayImpl() {
    GLCall(glDeleteVertexArrays(1, &m_VAOID));
}

void VertexArrayImpl::AddBuffer(const IVertexBuffer& vb, const IVertexLayout& layout) {
    this->Bind();  // 先绑定当前VAO

    // 动态转换并更新Layout的VAO引用
    try {
        auto& glLayout = dynamic_cast<const VertexLayoutImpl&>(layout);
        glLayout.UpdateVAO(m_VAOID);
    } catch (const std::bad_cast& e) {
        throw std::runtime_error("Incompatible layout type");
    }

    vb.Bind();
    layout.ApplyLayout();

    m_CurrentVBO = static_cast<const VertexBufferImpl&>(vb).GetID();
}

void VertexArrayImpl::Bind() const {
    GLCall(glBindVertexArray(m_VAOID));
    // 自动恢复VBO绑定状态
    if (m_CurrentVBO != 0) {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_CurrentVBO));
    }
}

void VertexArrayImpl::Unbind() const {
    GLCall(glBindVertexArray(0));
}

void VertexArrayImpl::Reset() {
    GLCall(glBindVertexArray(0));
    m_CurrentVBO = 0;  // 重置状态
}
}  // namespace OpenGLGraphics