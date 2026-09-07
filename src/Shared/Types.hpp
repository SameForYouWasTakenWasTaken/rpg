#pragma once

#include <filesystem>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace ssg
{

// STANDARD LIB ALIASES
using String = std::string;

template <typename T> using Vector = std::vector<T>;

template <typename T, size_t SIZE> using Array = std::array<T, SIZE>;

using Filepath = std::filesystem::path;

// SSG ALIASES
using Vec4 = glm::vec4;
using Vec3 = glm::vec3;
using Vec2 = glm::vec2;
using zIndex_t = std::uint8_t;
using TextureID = std::uint32_t;
using AtlasID = std::string;

// Inventory stuff
using InventorySlot_t = std::uint32_t;
using ItemCount_t = std::uint32_t;

} // namespace ssg