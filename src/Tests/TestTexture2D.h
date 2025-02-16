#pragma once

#include "OpenGLIndexBuffer.h"
#include "OpenGLRenderer.h"
#include "OpenGLVertexArray.h"
#include "OpenGLVertexBuffer.h"
#include "OpenGLVertexLayout.h"
#include "ResourceManager.hpp"
#include "Test.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "path_utils.h"
namespace test {
class TestTexture2D : public Test {
public:
    TestTexture2D(ShaderID shaderID,
                  TextureID textureID,
                  const std::filesystem::path& vsPath,
                  const std::filesystem::path& fsPath,
                  const std::filesystem::path& texturePath);
    ~TestTexture2D();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;

private:
    glm::vec3 m_TranslationA, m_TranslationB;
    glm::mat4 m_Proj, m_View;
    ShaderID m_ShaderID;
    TextureID m_TextureID;
    std::unique_ptr<IVertexArray> m_VAO;
    std::unique_ptr<IVertexBuffer> m_VBO;
    std::unique_ptr<IIndexBuffer> m_IBO;
    std::shared_ptr<Shader> m_Shader;
    std::shared_ptr<Texture> m_Texture;
};
}  // namespace test