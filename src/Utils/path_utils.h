#pragma once

#include <filesystem>
#include <string>

std::string GetExecutablePath();
std::string GetResourcePath(const std::filesystem::path& resource);