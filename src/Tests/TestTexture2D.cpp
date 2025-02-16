#include "TestTexture2D.h"

namespace test {
TestTexture2D::TestTexture2D(
    ShaderID shaderID, TextureID textureID, const std::filesystem::path& vsPath,
    const std::filesystem::path& fsPath, const std::filesystem::path& texturePath) :
    m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
    m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
    m_TranslationA(200, 200, 0),
    m_TranslationB(400, 200, 0),
    m_ShaderID(shaderID),
    m_TextureID(textureID) {
    float positions[] = {
        -50.0f, -50.0f, 0.0f, 0.0f,  // Bottom_left
         50.0f, -50.0f, 1.0f, 0.0f,  // Bottom_right
         50.0f,  50.0f, 1.0f, 1.0f,  // Top_right
        -50.0f,  50.0f, 0.0f, 1.0f  // Top_left
    };

    unsigned int indices[] = {0, 1, 2, 2, 3, 0};

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    // 使用ResourceManager加载核心资源
    auto [shader, shaderEc] = ResourceManager::LoadShader(shaderID, vsPath, fsPath);
    m_Shader = shader;

    auto [texture, texEc] = ResourceManager::LoadTexture(textureID, texturePath);
    m_Texture = texture;

    m_VAO = OpenGLGraphics::VertexArrayImpl::Create();
    m_VBO = OpenGLGraphics::VertexBufferImpl::Create(positions, 4 * 4 * sizeof(float));
    auto layout = OpenGLGraphics::VertexLayoutImpl::Create();
    layout->Push<float>(2);  // input the positions
    layout->Push<float>(2);  // input the texture coordinates
    m_VAO->AddBuffer(*m_VBO, *layout);

    m_IBO = OpenGLGraphics::IndexBufferImpl::Create(indices, 6);

    m_Shader->Bind();

    m_Shader->SetUniform1i("u_Texture", 0);
    // // Unbind all the bound resources, try to bind vertex vbo and element vbo everytime
}

TestTexture2D::~TestTexture2D() {
    m_Shader->Unbind();
    m_Texture->Unbind();

    ResourceManager::Unload(m_ShaderID);
    ResourceManager::Unload(m_TextureID);
}

void
TestTexture2D::OnUpdate(float deltaTime) {}

void
TestTexture2D::OnRender() {
    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    OpenGLRenderer renderer;
    m_Texture->Bind(0);

    {
        glm::mat4 model1 = glm::translate(glm::mat4(1.0f), m_TranslationA);
        glm::mat4 mvp = m_Proj * m_View * model1;
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", &mvp[0][0]);

        renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
    }

    {
        glm::mat4 model2 = glm::translate(glm::mat4(1.0f), m_TranslationB);
        glm::mat4 mvp = m_Proj * m_View * model2;
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", &mvp[0][0]);

        renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
    }
}

void
TestTexture2D::OnImGuiRender() {
    ImGui::SliderFloat3("TranslationA ##A", &m_TranslationA.x, 0.0f, 960.0f);
    ImGui::SliderFloat3("TranslationB ##B", &m_TranslationB.x, 0.0f, 960.0f);
    ImGui::Text(
        "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
        ImGui::GetIO().Framerate);
}
}  // namespace test