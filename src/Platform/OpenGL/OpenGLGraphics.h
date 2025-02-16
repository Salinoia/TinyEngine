#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"

namespace OpenGLGraphics {
using GLuint = GLuint;
enum class BufferUsage {
    StaticDraw = GL_STATIC_DRAW,
    DynamicDraw = GL_DYNAMIC_DRAW,
    StreamDraw = GL_STREAM_DRAW
};
}  // namespace OpenGLGraphics