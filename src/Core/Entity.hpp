#pragma once

#include <entt/entt.hpp>
#include <type_traits>

namespace ssg
{

class Entity
{
  public:
    using entity_type = entt::entity;
    using underlying_type = std::underlying_type_t<entity_type>;

    constexpr Entity() noexcept = default;

    constexpr Entity(entity_type value) noexcept : id(value) {}

    constexpr Entity(underlying_type value) noexcept : id(static_cast<entity_type>(value)) {}

    constexpr operator entity_type() const noexcept { return id; }

    constexpr operator underlying_type() const noexcept { return static_cast<underlying_type>(id); }

    friend constexpr bool operator==(const Entity& lhs, const Entity& rhs) noexcept
    {
        return lhs.id == rhs.id;
    }

    friend constexpr bool operator==(const Entity& lhs, entt::entity rhs) noexcept
    {
        return lhs.id == rhs;
    }

    friend constexpr bool operator==(entt::entity lhs, const Entity& rhs) noexcept
    {
        return lhs == rhs.id;
    }

    friend constexpr bool operator!=(const Entity& lhs, const Entity& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    friend constexpr bool operator!=(const Entity& lhs, entt::entity rhs) noexcept
    {
        return !(lhs == rhs);
    }

    friend constexpr bool operator!=(entt::entity lhs, const Entity& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    entity_type id{entt::null};
};

} // namespace ssg