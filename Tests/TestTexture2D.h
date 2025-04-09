#pragma once

#include "Renderer.h"
#include "Test.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "path_ultis.h"
namespace test {
class TestTexture2D : public Test {
public:
    TestTexture2D(const std::string& shaderPath, const std::string& texturePath);
    ~TestTexture2D();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;

private:
    glm::vec3 m_TranslationA, m_TranslationB;
    glm::mat4 m_Proj, m_View;
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VBO;
    std::unique_ptr<IndexBuffer> m_IBO;
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<Texture> m_Texture;
};
}  // namespace test