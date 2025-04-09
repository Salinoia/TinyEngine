#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "Shader.h"
#include "TestClearColor.h"
#include "TestTexture2D.h"
#include "Texture.h"

int main() {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

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
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        Renderer renderer;

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        std::unique_ptr<test::Test> currentTest = nullptr;

        test::TestMenu::RegisterTest<test::TestClearColor>("Clear Color");
        test::TestMenu::RegisterTest<test::TestTexture2D>("2D Texture", "res/shaders/Basic.shader", "res/textures/UN.png");

        currentTest = std::make_unique<test::TestMenu>(currentTest);
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
            renderer.Clear();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (currentTest) {
                currentTest->OnUpdate(0.0f);
                currentTest->OnRender();
                ImGui::Begin("Test");
                bool isMainMenu = dynamic_cast<test::TestMenu*>(currentTest.get()) != nullptr;
                if (!isMainMenu && ImGui::Button("<-")) {
                    currentTest = std::make_unique<test::TestMenu>(currentTest);
                }

                currentTest->OnImGuiRender();
                ImGui::End();
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
