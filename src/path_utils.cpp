#include <limits.h>
#include <iostream>
#include <vector>
#include "path_ultis.h"


#ifdef _WIN32
#include <windows.h>
#include <wchar.h>
#include <locale>
#include <codecvt>
#else
#include <unistd.h>
#include <limits.h>
#endif

std::string GetExecutablePath() {
#ifdef _WIN32
    wchar_t path[MAX_PATH];
    DWORD result = GetModuleFileNameW(NULL, path, MAX_PATH);
    if (result == 0 || result >= MAX_PATH) {
        std::cerr << "Failed to get executable path!" << std::endl;
        return "";
    }
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(path);
#else
    std::vector<char> path(4096);
    ssize_t count = readlink("/proc/self/exe", path.data(), path.size());
    while (count == static_cast<ssize_t>(path.size())) {
        path.resize(path.size() * 2);
        count = readlink("/proc/self/exe", path.data(), path.size());
    }
    if (count == -1) {
        std::cerr << "Failed to get executable path!" << std::endl;
        return "";
    }
    path[count] = '\0';
    return std::string(path.data());
#endif
}

#if __cplusplus >= 201703L
#include <filesystem>
#endif

std::string GetResourcePath(const std::string& resource) {
#if __cplusplus >= 201703L
    std::filesystem::path exePath = GetExecutablePath();
    std::filesystem::path resourcePath = exePath.parent_path() / ".." / resource;
    return resourcePath.lexically_normal().string();
#else
    std::string exePath = GetExecutablePath();
    size_t lastSlash = exePath.find_last_of('/\\');     // 支持 Windows 和 Unix 路径分隔符
    std::string exeDir = exePath.substr(0, lastSlash);  // 获取可执行文件所在目录
    return exeDir + "/../" + resource;                  // 返回资源文件的绝对路径
#endif
}
