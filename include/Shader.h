#pragma once
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader {
private:
    std::string m_Filepath;
    unsigned int m_RendererID;
    // Caching for uniforms
    mutable std::unordered_map<std::string, GLint> m_UniformLocationCache;

public:
    Shader(const std::string& filepath);
    ~Shader();

    void Bind() const;
    void Unbind() const;
    bool IsBound() const;

    // Set uniforms
    void SetUniform1i(const std::string& name, int value);  // sampler slot, not a integer uniform
    void SetUniform1f(const std::string& name, float value);
    void SetUniform2f(const std::string& name, const glm::vec2& value);
    void SetUniform3f(const std::string& name, const glm::vec3& value);
    void SetUniform4f(const std::string& name, const glm::vec4& value);

    void SetUniformMat3f(const std::string& name, const glm::mat3& matrix);
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
    ShaderProgramSource ParseShader(const std::string& filepath);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    GLint GetUniformLocation(const std::string& name) const;
};