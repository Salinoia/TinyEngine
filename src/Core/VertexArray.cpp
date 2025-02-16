#include "VertexArray.h"

#include "OpenGLVertexArray.h"

std::unique_ptr<IVertexArray> IVertexArray::Create() {
    return std::make_unique<OpenGLGraphics::VertexArrayImpl>();
}
