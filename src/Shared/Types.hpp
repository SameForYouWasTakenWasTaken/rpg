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
using AtlasID = std::string;

// SSG TYPES
struct TextureHandle
{
    using value_type = std::uint32_t;
    value_type id;

    TextureHandle() = default;
    ~TextureHandle() = default;
    TextureHandle(value_type id) : id(id) {}

    [[nodiscard]] bool IsValid() const { return id != static_cast<value_type>(-1); }
    [[nodiscard]] static value_type GetInvalidValue() { return static_cast<value_type>(-1); }

    friend bool operator==(const TextureHandle& lhs, const TextureHandle& rhs)
    {
        return lhs.id == rhs.id;
    }
    friend bool operator!=(const TextureHandle& lhs, const TextureHandle& rhs)
    {
        return !(lhs == rhs);
    }
    explicit operator value_type() const { return id; }
};

// Inventory stuff
using InventorySlot_t = std::uint32_t;
using ItemCount_t = std::uint32_t;

} // namespace ssg