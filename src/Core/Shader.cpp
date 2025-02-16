#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "OpenGLRenderer.h"
#include "glm/glm.hpp"
#include "path_utils.h"

namespace OpenGLGraphics {
class OpenGLShader final : public Shader {};
}  // namespace OpenGLGraphics

Shader::Shader(const std::filesystem::path& vsPath, const std::filesystem::path& fsPath) :
    m_RendererID(0), m_VSPath(vsPath), m_FSPath(fsPath) {
    // 获取完整资源路径
    const auto vertFullPath = GetResourcePath(vsPath);
    const auto fragFullPath = GetResourcePath(fsPath);

    // 使用ParseShader加载shader代码
    const ShaderProgramSource sources = ParseShader(vertFullPath, fragFullPath);
    m_RendererID = CreateShader(sources.VertexSource, sources.FragmentSource);
}

Shader::~Shader() {
    GLCall(glDeleteProgram(m_RendererID));
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        if (length <= 0 || length > 8388608) {
            // 处理非法长度的情况
            std::cerr << "Invalid log length: " << length << std::endl;
            return 0;
        }
        char* message = (char*) alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexsrc, const std::string& fragmentsrc) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexsrc);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentsrc);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void Shader::Bind() const {
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const {
    GLCall(glUseProgram(0));
}

bool Shader::IsBound() const {
    GLint currentProgram = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    return currentProgram == static_cast<GLint>(m_RendererID);
}

void Shader::SetUniform1i(const std::string& name, int value) {
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value) {
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform2f(const std::string& name, float x, float y) {
    GLCall(glUniform2f(GetUniformLocation(name), x, y));
}

void Shader::SetUniform3f(const std::string& name, float x, float y, float z) {
    GLCall(glUniform3f(GetUniformLocation(name), x, y, z));
}

void Shader::SetUniform4f(const std::string& name, float x, float y, float z, float w) {
    GLCall(glUniform4f(GetUniformLocation(name), x, y, z, w));
}

void Shader::SetUniformMat3f(const std::string& name, const float* matrix) {
    GLCall(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, matrix));
}

void Shader::SetUniformMat4f(const std::string& name, const float* matrix) {
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, matrix));
}

std::string Shader::ReadFile(const std::filesystem::path& path)  // 参数改为filesystem::path
{
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Shader file not found: " + path.string());
    }

    std::string content;
    try {
        std::stringstream ss;
        ss << file.rdbuf();
        content = ss.str();
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to read shader file: " + path.string()
                                 + "\nError: " + e.what());
    }

    if (content.empty()) {
        throw std::runtime_error("Empty shader file: " + path.string());
    }
    return content;
}

ShaderProgramSource Shader::ParseShader(const std::string& vsPath, const std::string& fsPath) {
    // // 分别读取两个文件
    // auto LoadShaderFile = [](const std::string& path) -> std::string {
    //     std::ifstream file(path);
    //     if (!file.is_open()) {
    //         throw std::runtime_error("Failed to open shader file: " + path);
    //     }
    //     return std::string(std::istreambuf_iterator<char>(file),
    //     std::istreambuf_iterator<char>());
    // };

    return {
        ReadFile(vsPath),  // 顶点着色器源码
        ReadFile(fsPath)   // 片段着色器源码
    };
}

int Shader::GetUniformLocation(const std::string& name) const {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        return m_UniformLocationCache[name];
    }
    int location;
    GLCall(location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1) {
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
    }
    m_UniformLocationCache[name] = location;
    return location;
}
