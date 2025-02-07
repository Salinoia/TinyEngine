#include "Renderer.h"
#include <iostream>

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

void Renderer::Clear() const { GLCall(glClear(GL_COLOR_BUFFER_BIT)); }

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    if(!shader.IsBound()){
        shader.Bind();
    }
    va.Bind();
    ib.Bind();
    // The most useful function of OpenGL
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}