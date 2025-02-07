#include "Test.h"

#include "imgui.h"

namespace test {
    TestMenu::TestMenu(std::unique_ptr<Test>& currentTestPointer) : m_CurrentTest(currentTestPointer) {}

    void TestMenu::OnImGuiRender() {
        for (auto& [name, func] : TestRegistry::s_Tests) {
            if (ImGui::Button(name.c_str())) {
                m_CurrentTest = func();
            }
        }
    }
}  // namespace test