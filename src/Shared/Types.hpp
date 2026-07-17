#pragma once

#include <string>
#include <filesystem>
#include <vector>

#include <glm/glm.hpp>

namespace ssg
{
    using String = std::string;

    template<typename T>
    using Vector = std::vector<T>;

    using Vec3 = glm::vec3;
    using Vec2 = glm::vec2;
    using Filepath = std::filesystem::path;
}