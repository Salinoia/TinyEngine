#pragma once

#include <GL/glew.h>

#include <unordered_map>
#include <vector>

struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned int normalized;

    static int SizeofGLTypes(unsigned int gl_type) {
        static const std::unordered_map<unsigned int, int> type_sizes = {
            {GL_BYTE, sizeof(GLbyte)},  // 1 byte
            {GL_UNSIGNED_BYTE, sizeof(GLubyte)},  // 1 byte
            {GL_SHORT, sizeof(GLshort)},  // 2 bytes
            {GL_UNSIGNED_SHORT, sizeof(GLushort)},  // 2 bytes
            {GL_INT, sizeof(GLint)},  // 4 bytes
            {GL_UNSIGNED_INT, sizeof(GLuint)},  // 4 bytes
            {GL_FLOAT, sizeof(GLfloat)},  // 4 bytes
            {GL_DOUBLE, sizeof(GLdouble)},  // 8 bytes
            {GL_HALF_FLOAT, 2},  // OpenGL half float (16-bit)
            {GL_FIXED, 4}  // 4 bytes (used in OpenGL ES)
        };

        auto it = type_sizes.find(gl_type);
        if (it != type_sizes.end()) {
            return it->second;
        }

        return -1;  // Unknown type
    }

    VertexBufferElement(unsigned int t, unsigned int c, bool n) : type(t), count(c), normalized(n) {}
};

class VertexBufferLayout {
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;

public:
    VertexBufferLayout() : m_Stride(0) {}
    ~VertexBufferLayout() {}

    // 主模板
    template <typename T>
    void Push(unsigned int count) {
        static_assert(sizeof(T) == 0, "Unsupported type");
    }

    inline const std::vector<VertexBufferElement> GetElements() const& { return m_Elements; }
    inline unsigned int GetStride() const { return m_Stride; }
};

// 成员函数模板特化（必须在类外）
template <>
inline void VertexBufferLayout::Push<float>(unsigned int count) {
    m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
    m_Stride += VertexBufferElement::SizeofGLTypes(GL_FLOAT) * count;
}

template <>
inline void VertexBufferLayout::Push<unsigned int>(unsigned int count) {
    m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
    m_Stride += VertexBufferElement::SizeofGLTypes(GL_UNSIGNED_INT) * count;
}

template <>
inline void VertexBufferLayout::Push<unsigned char>(unsigned int count) {
    m_Elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
    m_Stride += VertexBufferElement::SizeofGLTypes(GL_UNSIGNED_BYTE) * count;
}
