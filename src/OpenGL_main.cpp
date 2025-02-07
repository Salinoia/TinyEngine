#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "path_ultis.h"
int main() {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) return -1;

    // Running in core profile and
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Hello OpenGL", NULL, NULL);
    glfwSwapInterval(1);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    GLCall(glfwMakeContextCurrent(window));

    /* Initialize GLEW */
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW!" << std::endl;
        return -1;
    }

    std::cout << "OpenGL Version: \n" << glGetString(GL_VERSION) << std::endl;

    {
        float positions[] = {
            -50.0f, -50.0f, 0.0f, 0.0f,  // Bottom_left
            50.0f,  -50.0f, 1.0f, 0.0f,  // Bottom_right
            50.0f,  50.0f,  1.0f, 1.0f,  // Top_right
            -50.0f, 50.0f,  0.0f, 1.0f   // Top_left
        };

        unsigned int indices[] = {0, 1, 2, 2, 3, 0};

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);  // input the positions
        layout.Push<float>(2);  // input the texture coordinates
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        // glm::mat4 proj = glm::ortho(-1.0f, 1.0f, -0.75f, 0.75f, -1.0f, 1.0f);
        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        Texture texture("res/textures/Google.png");
        texture.Bind(0);
        shader.SetUniform1i("u_Texture", 0);
        // Unbind all the bound resources, try to bind vertex vbo and element vbo everytime
        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsLight();

        float r = 0.0f;
        float increment = 0.01f;
        glm::vec3 translationA(200.0f, 200.0f, 0.0f);
        glm::vec3 translationB(400.0f, 200.0f, 0.0f);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            /* Render here */
            renderer.Clear();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            {
                glm::mat4 model1 = glm::translate(glm::mat4(1.0f), translationA);
                glm::mat4 mvp = proj * view * model1;
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);

                renderer.Draw(va, ib, shader);
            }

            {
                glm::mat4 model2 = glm::translate(glm::mat4(1.0f), translationB);
                glm::mat4 mvp = proj * view * model2;
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);

                renderer.Draw(va, ib, shader);
            }

            // instead of uniforms, batching renderering is much more faster by only using one
            // vertex buffer that's something eventually called a material which contains a bunch of
            // uniforms and a shader
            {
                ImGui::SliderFloat3("TranslationA ##A", &translationA.x, 0.0f, 960.0f);
                ImGui::SliderFloat3("TranslationB ##B", &translationB.x, 0.0f, 960.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate,
                            io.Framerate);
            }
            // Rendering
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
