#include "OpenGLRenderer.h"
#include <iostream>

IRenderer::~IRenderer() = default;

 void GLClearError(){
    while(glGetError() != GL_NO_ERROR);
}

const char* GetGLErrorString(GLenum error) {
    switch (error) {
        case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
        case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
        default: return "Unknown Error";
    }
}

 bool GLLogCall(const char* function, const char* file, int line){
    bool HashError = false;
    while(GLenum error = glGetError()){
        std::cout << "[OpenGL Error](" << GetGLErrorString(error) << "): " << function << 
        " " << file << ":" << line << std::endl;
        HashError = true;
    }
    return !HashError;
}

void OpenGLRenderer::Clear() const { GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)); }

void OpenGLRenderer::Draw(const IVertexArray& va, const IIndexBuffer& ib, const Shader& shader) const {
    if(!shader.IsBound()){
        shader.Bind();
    }
    va.Bind();
    ib.Bind();
    // The most useful function of OpenGL
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

// OpenGLRenderer.cpp（补充实现）
void OpenGLRenderer::SetColor(float r, float g, float b, float a) const {
    GLCall(glClearColor(r, g, b, a));
}

void OpenGLRenderer::EnableDepthTest(bool enable) {
    GLCall(enable ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST));
}

void OpenGLRenderer::SetViewport(int x, int y, int w, int h) {
    GLCall(glViewport(x, y, w, h));
}