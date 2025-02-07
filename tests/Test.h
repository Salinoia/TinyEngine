#pragma once
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <utility>  // for std::forward
#include <vector>

namespace test {
// 全局测试注册表
class Test {
   public:
    Test() = default;
    virtual ~Test() = default;
    // why not pure virtual function?
    // leave a choice to decide wheth implement it or not
    virtual void OnUpdate(float deltaTime) {}
    virtual void OnRender() {}
    virtual void OnImGuiRender() {}
};
struct TestRegistry {
    static std::vector<std::pair<std::string, std::function<std::unique_ptr<Test>()>>> s_Tests;
};
class TestMenu : public Test {
   public:
    TestMenu(std::unique_ptr<Test>& currentTestPointer);
    void OnImGuiRender() override;

    // template<typename T>
    // void RegisterTest(const std::string& name){
    //     std::cout << "Registering Test: " << name << std::endl;
    //     m_Tests.push_back(std::make_pair(name, [](){
    //         return std::make_unique<T>();
    //     }));
    // }
    // 注册测试（全局生效）
    template <typename T, typename... Args>
    static void RegisterTest(const std::string& name, Args&&... args) {
        std::cout << "Registering Test: " << name << std::endl;
        TestRegistry::s_Tests.push_back(std::make_pair(name, [args...]() mutable {
            return std::make_unique<T>(std::forward<Args>(args)...);
        }));
    }

   private:
    std::unique_ptr<Test>& m_CurrentTest;
    // std::vector<std::pair<std::string, std::function<std::unique_ptr<Test>()>>> m_Tests;
};
inline std::vector<std::pair<std::string, std::function<std::unique_ptr<Test>()>>> TestRegistry::s_Tests;
}  // namespace test
