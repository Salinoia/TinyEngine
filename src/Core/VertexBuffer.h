#pragma once
#include <memory>

class IVertexBuffer {
public:
    virtual ~IVertexBuffer() = default;

    // 核心接口
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    // 工厂方法
    static std::unique_ptr<IVertexBuffer> Create(const void* data, unsigned int size);

    // 禁用拷贝（保护资源）
    IVertexBuffer(const IVertexBuffer&) = delete;
    IVertexBuffer& operator=(const IVertexBuffer&) = delete;

protected:
    IVertexBuffer(IVertexBuffer&&) = default;
    IVertexBuffer& operator=(IVertexBuffer&&) = default;
    IVertexBuffer() = default;
};
