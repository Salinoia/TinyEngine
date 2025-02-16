#pragma once
#include <memory>
#include <type_traits>
#include <vector>

struct VertexAttributeDescriptor {
    size_t typeSize;        // 类型字节大小
    unsigned int count;     // 元素数量
    bool normalized;        // 是否归一化
    unsigned int typeHash;  // 类型哈希标识
};

class IVertexLayout {
public:
    template <typename T>
    void Push(unsigned int count, bool normalized = false) {
        static_assert(std::is_standard_layout_v<T>,
                      "Vertex attribute must be standard layout type");
        DoPush(sizeof(T),
               count,
               normalized,
               typeid(T).hash_code()  // 跨平台类型标识
        );
    }
    virtual ~IVertexLayout() = default;

    virtual size_t GetStride() const noexcept = 0;
    virtual const std::vector<VertexAttributeDescriptor>& GetAttributes() const noexcept = 0;
    virtual void ApplyLayout() const = 0;

    // 工厂方法
    static std::unique_ptr<IVertexLayout> Create();

protected:
    virtual void DoPush(size_t typeSize, unsigned int count, bool normalized, size_t typeHash) = 0;
};
