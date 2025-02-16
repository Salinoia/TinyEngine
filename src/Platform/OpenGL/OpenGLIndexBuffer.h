#pragma once
#include "IndexBuffer.h"

namespace OpenGLGraphics {

class IndexBufferImpl final : public IIndexBuffer {
private:
    unsigned int m_RendererID;
    unsigned int m_Count;

public:
    IndexBufferImpl(const unsigned int* data, unsigned int count);
    ~IndexBufferImpl() override;

    // 接口实现
    void Bind() const override;
    void Unbind() const override;
    unsigned int GetCount() const noexcept override { return m_Count; }
};

}  // namespace OpenGLGraphics
