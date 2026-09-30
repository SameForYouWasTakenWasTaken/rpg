#pragma once
#include <concepts>
#include <string_view>

// The vocabulary used to describe a scriptable component.
// No sol2 or entt in here: this header is cheap to include.

namespace ssg::lua
{
template <class... Ts> struct TypeList
{
};

// One exposed member: its Lua-facing name and a pointer to the C++ member.
template <class C, class T> struct Field
{
    const char* name;
    T C::* ptr;
};
template <class C, class T> Field(const char*, T C::*) -> Field<C, T>;

// Specialize once per scriptable component (see ScriptComponents.hpp).
// A specialization provides:
//   static constexpr std::string_view name;   // Lua name, snake_case
//   static constexpr auto fields;             // std::tuple of Field{...}
template <class C> struct ScriptComponent;

// Gives a readable error when a component is listed but never described.
template <class C>
concept Scriptable = requires {
    { ScriptComponent<C>::name } -> std::convertible_to<std::string_view>;
    ScriptComponent<C>::fields;
};
} // namespace ssg::lua