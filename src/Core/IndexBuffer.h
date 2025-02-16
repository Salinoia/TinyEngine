#pragma once
#include <memory>  // 添加智能指针支持

class IIndexBuffer {
public:
    virtual ~IIndexBuffer() = default;

    // 核心接口
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual unsigned int GetCount() const noexcept = 0;  // 纯虚版本

    // 工厂方法
    static std::unique_ptr<IIndexBuffer> Create(const unsigned int* data, unsigned int count);

    // 资源管理
    IIndexBuffer(const IIndexBuffer&) = delete;
    IIndexBuffer& operator=(const IIndexBuffer&) = delete;

protected:
    // 允许子类移动操作
    IIndexBuffer(IIndexBuffer&&) = default;
    IIndexBuffer& operator=(IIndexBuffer&&) = default;
    IIndexBuffer() = default;
};
