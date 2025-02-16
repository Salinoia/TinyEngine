#pragma once
#include <GL/glew.h>

#include "VertexLayout.h"

namespace OpenGLGraphics {
class VertexArrrayImpl;  // 前向声明
class VertexLayoutImpl final : public IVertexLayout {
    friend class VertexArrayImpl;

public:
    VertexLayoutImpl();
    ~VertexLayoutImpl();

    size_t GetStride() const noexcept override;
    const std::vector<VertexAttributeDescriptor>& GetAttributes() const noexcept override;
    void ApplyLayout() const override;

private:
    void UpdateVAO(GLuint targetVAO) const;
    void DoPush(size_t typeSize, unsigned int count, bool normalized, size_t typeHash) override;

    // OpenGL类型映射
    static GLenum MapGLType(size_t typeSize, size_t typeHash);

    std::vector<VertexAttributeDescriptor> m_Attributes;
    size_t m_CalculatedStride = 0;
    mutable GLuint m_CurrentVAO = 0;  // 延迟绑定用
};
}  // namespace OpenGLGraphics