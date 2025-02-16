#pragma once

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"

class IRenderer {
public:
    virtual ~IRenderer();

    virtual void Clear() const = 0;
    virtual void SetColor(float r, float g, float b, float a) const = 0;
    virtual void Draw(const IVertexArray& va,
                      const IIndexBuffer& ib,
                      const Shader& shader) const = 0;

    virtual void EnableDepthTest(bool enable) = 0;
    virtual void SetViewport(int x, int y, int width, int height) = 0;
};