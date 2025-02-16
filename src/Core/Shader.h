#pragma once
#include <filesystem>
#include <string>
#include <unordered_map>

#include "ResourceType.hpp"

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader {
public:
    using ResourceIDType = ShaderID;

    // 修改后的构造函数
    explicit Shader(const std::filesystem::path& vsPath, const std::filesystem::path& fsPath);

    // 删除拷贝操作
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    // 移动构造函数
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    ~Shader();

    // 内存使用统计接口
    size_t GetMemoryUsage() const noexcept {
        return m_ShaderSources.VertexSource.size() + m_ShaderSources.FragmentSource.size();
    }

    // 着色器操作接口
    void Bind() const;
    void Unbind() const;
    bool IsBound() const;

    // Uniform设置方法
    void SetUniform1i(const std::string& name, int value);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform2f(const std::string& name, float x, float y);
    void SetUniform3f(const std::string& name, float x, float y, float z);
    void SetUniform4f(const std::string& name, float x, float y, float z, float w);
    void SetUniformMat3f(const std::string& name, const float* matrixData);
    void SetUniformMat4f(const std::string& name, const float* matrixData);

private:
    std::filesystem::path m_VSPath;
    std::filesystem::path m_FSPath;
    ShaderProgramSource m_ShaderSources;
    unsigned int m_RendererID = 0;
    mutable std::unordered_map<std::string, int> m_UniformLocationCache;

    std::string ReadFile(const std::filesystem::path& path);
    unsigned int CreateShader(const std::string& vsPath, const std::string& fsPath);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    ShaderProgramSource ParseShader(const std::string& vsPath, const std::string& fsPath);
    int GetUniformLocation(const std::string& name) const;
};