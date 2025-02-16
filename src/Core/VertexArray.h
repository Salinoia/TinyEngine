#pragma once
#include <memory>

#include "VertexBuffer.h"
#include "VertexLayout.h"

class IVertexArray {
public:
    // 核心接口
    virtual void AddBuffer(const IVertexBuffer& vb, const IVertexLayout& layout) = 0;
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    // 工厂方法
    static std::unique_ptr<IVertexArray> Create();

    // 资源管理
    IVertexArray(const IVertexArray&) = delete;
    IVertexArray& operator=(const IVertexArray&) = delete;

    virtual ~IVertexArray() = default;

protected:
    IVertexArray() = default;
    IVertexArray(IVertexArray&&) = default;
    IVertexArray& operator=(IVertexArray&&) = default;
};
