#include "IndexBuffer.h"

#include "OpenGLIndexBuffer.h"

std::unique_ptr<IIndexBuffer> IIndexBuffer::Create(const unsigned int* data, unsigned int count) {
    return std::make_unique<OpenGLGraphics::IndexBufferImpl>(data, count);
}