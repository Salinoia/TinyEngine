#pragma once
#include <GL/glew.h>

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"

#if defined(_MSC_VER)
#    define DEBUG_BREAK() __debugbreak()
#elif defined(__clang__) || defined(__GNUC__)
// #define DEBUG_BREAK() __builtin_trap()
#    include <csignal>
#    define DEBUG_BREAK() raise(SIGTRAP)
#else
#    include <cstdlib>
#    define DEBUG_BREAK() abort()
#endif

#define ASSERT(x) \
    if (!(x))     \
        DEBUG_BREAK();

#define GLCall(x)                                  \
    do {                                           \
        GLClearError();                            \
        x;                                         \
        ASSERT(GLLogCall(#x, __FILE__, __LINE__)); \
    } while (0)

void GLClearError();

const char* GetGLErrorString(GLenum error);

bool GLLogCall(const char* function, const char* file, int line);

class Renderer {
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};
