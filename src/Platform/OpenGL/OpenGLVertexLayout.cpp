#include "OpenGLVertexLayout.h"

#include "OpenGLRenderer.h"

namespace OpenGLGraphics {

// 移除实现逻辑，将VAO的管理权交给外部传入
VertexLayoutImpl::VertexLayoutImpl() : m_CurrentVAO(0) {}

VertexLayoutImpl::~VertexLayoutImpl() {}

GLenum VertexLayoutImpl::MapGLType(size_t typeSize, size_t typeHash) {
    static const std::unordered_map<size_t, GLenum> typeMap = {
        {typeid(float).hash_code(), GL_FLOAT},
        {typeid(int).hash_code(), GL_INT},
        {typeid(unsigned int).hash_code(), GL_UNSIGNED_INT}};

    auto it = typeMap.find(typeHash);
    if (it != typeMap.end())
        return it->second;

    switch (typeSize) {
    case 1:
        return GL_UNSIGNED_BYTE;
    case 2:
        return GL_SHORT;
    case 4:
        return GL_FLOAT;  // 默认fallback
    default:
        throw std::runtime_error("Unsupported vertex attribute type");
    }
}

void VertexLayoutImpl::DoPush(size_t typeSize,
                              unsigned int count,
                              bool normalized,
                              size_t typeHash) {
    const GLenum glType = MapGLType(typeSize, typeHash);

    m_Attributes.push_back({
        typeSize,
        count,
        normalized,
        static_cast<unsigned int>(glType)  // 存储OpenGL类型标识
    });

    // 重新计算步长
    m_CalculatedStride += count * typeSize;
}

size_t VertexLayoutImpl::GetStride() const noexcept {
    return m_CalculatedStride;
}

const std::vector<VertexAttributeDescriptor>& VertexLayoutImpl::GetAttributes() const noexcept {
    return m_Attributes;
}

void VertexLayoutImpl::ApplyLayout() const {
    if (m_CurrentVAO == 0) {
        throw std::runtime_error("VAO not initialized in IVertexLayout");
    }
    if (glIsVertexArray(m_CurrentVAO) == GL_FALSE) {
        throw std::runtime_error("Invalid VAO in layout application");
    }

    GLCall(glBindVertexArray(m_CurrentVAO));

    size_t offset = 0;
    for (unsigned int i = 0; i < m_Attributes.size(); ++i) {
        const auto& attr = m_Attributes[i];
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i,
                                     attr.count,
                                     static_cast<GLenum>(attr.typeHash),  // 存储的OpenGL类型
                                     attr.normalized ? GL_TRUE : GL_FALSE,
                                     m_CalculatedStride,
                                     reinterpret_cast<const void*>(offset)));
        offset += attr.count * attr.typeSize;
    }
}

void VertexLayoutImpl::UpdateVAO(GLuint targetVAO) const {
    m_CurrentVAO = targetVAO;
}

}  // namespace OpenGLGraphics