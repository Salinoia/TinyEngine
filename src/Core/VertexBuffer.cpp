#include "VertexBuffer.h"

#include "OpenGLVertexBuffer.h"

std::unique_ptr<IVertexBuffer> IVertexBuffer::Create(const void* data, unsigned int size) {
    return std::make_unique<OpenGLGraphics::VertexBufferImpl>(
        data, size, OpenGLGraphics::BufferUsage::StaticDraw);
}