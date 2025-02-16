#include "VertexLayout.h"

#include "OpenGLVertexLayout.h"

std::unique_ptr<IVertexLayout> IVertexLayout::Create() {
    return std::make_unique<OpenGLGraphics::VertexLayoutImpl>();
}