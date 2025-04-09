#pragma once
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <utility>  // for std::forward
#include <vector>

namespace test {
class Test {
public:
    Test() = default;
    virtual ~Test() = default;
    // why not pure virtual function?
    // leave a choice to decide whether implement it or not
    virtual void OnUpdate(float deltaTime) {}
    virtual void OnRender() {}
    virtual void OnImGuiRender() {}
};
// 全局测试注册表
struct TestRegistry {
    static inline std::vector<std::pair<std::string, std::function<std::unique_ptr<Test>()>>> s_Tests;
};
class TestMenu : public Test {
public:
    TestMenu(std::unique_ptr<Test>& currentTestPointer);
    void OnImGuiRender() override;

    // 非完美转发的基础模板
    template <typename T>
    static void RegisterTest(const std::string& name) {
        std::cout << "Registering Test: " << name << std::endl;
        TestRegistry::s_Tests.push_back(std::make_pair(name, []() { return std::make_unique<T>(); }));
    }

    // 完美转发的通用模板
    template <typename T, typename... Args>
    static void RegisterTest(const std::string& name, Args&&... args) {
        std::cout << "Registering Test: " << name << std::endl;
        TestRegistry::s_Tests.push_back(std::make_pair(name, [=]() mutable { return std::make_unique<T>(std::forward<Args>(args)...); }));
    }

private:
    std::unique_ptr<Test>& m_CurrentTest;
    // std::vector<std::pair<std::string, std::function<std::unique_ptr<Test>()>>> m_Tests;
};
}  // namespace test
