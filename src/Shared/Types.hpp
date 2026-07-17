#pragma once

#include <string>
#include <filesystem>
#include <vector>

#include <glm/glm.hpp>

namespace ssg
{

    // SSG ALIASES
    using Vec3 = glm::vec3;
    using Vec2 = glm::vec2;
    using zIndex_t = std::uint8_t;
    using TextureID = std::uint32_t;

    // STANDARD LIB ALIASES
    using String = std::string;

    template<typename T>
    using Vector = std::vector<T>;

    template <typename T, size_t SIZE>
    using Array = std::array<T, SIZE>;

    using Filepath = std::filesystem::path;
}