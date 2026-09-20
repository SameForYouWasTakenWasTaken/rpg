#pragma once
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>
#include <sol/sol.hpp>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>

#include "ScriptComponents.hpp"

namespace ssg::lua
{

struct ComponentOps
{
    void (*add)(entt::handle, const sol::table&);
    bool (*has)(entt::handle);
    void (*remove)(entt::handle);
    sol::object (*get)(entt::handle, sol::this_state);
};

// Getting components
template <class C> struct ComponentRef
{
    entt::handle handle;

    C& get() const
    {
        if (!handle.valid() || !handle.all_of<C>())
            throw std::runtime_error("component no longer exists");
        return handle.get<C>();
    }
};

template <class C, class T>
void bind_field(sol::usertype<ComponentRef<C>>& ut, const Field<C, T>& f)
{
    auto ptr = f.ptr;
    ut[f.name] = sol::property([ptr](const ComponentRef<C>& r) -> T { return r.get().*ptr; },
                               [ptr](ComponentRef<C>& r, const T& v) { r.get().*ptr = v; });
}

template <class C> void register_component_ref(sol::state& lua)
{
    auto ut = lua.new_usertype<ComponentRef<C>>(std::string(ScriptComponent<C>::name) + "Ref",
                                                sol::no_constructor);
    std::apply([&](const auto&... f) { (bind_field<C>(ut, f), ...); }, ScriptComponent<C>::fields);
}

template <class... Ts> void register_component_refs(sol::state& lua, TypeList<Ts...>)
{
    (register_component_ref<Ts>(lua), ...);
}

// Finding/Creating components
template <class C> C from_table(const sol::table& t)
{
    C c{};
    std::apply([&](const auto&... f) { ((c.*(f.ptr) = t.get_or(f.name, c.*(f.ptr))), ...); },
               ScriptComponent<C>::fields);
    return c;
}

template <class C> ComponentOps make_ops()
{
    return {
        [](entt::handle h, const sol::table& t) { h.emplace_or_replace<C>(from_table<C>(t)); },
        [](entt::handle h) { return h.all_of<C>(); },
        [](entt::handle h) { h.remove<C>(); },
        [](entt::handle h, sol::this_state ts) -> sol::object
        {
            if (!h.all_of<C>())
                return sol::make_object(ts, sol::lua_nil);
            return sol::make_object(ts, ComponentRef<C>{h});
        },
    };
}

template <class... Ts> auto build_ops(TypeList<Ts...>)
{
    std::unordered_map<std::string_view, ComponentOps> m;
    (m.emplace(ScriptComponent<Ts>::name, make_ops<Ts>()), ...);
    return m;
}

inline const ComponentOps& find_ops(std::string_view name)
{
    static const auto table = build_ops(ScriptComponents{}); // built once, on first call
    auto it = table.find(name);
    if (it == table.end())
        throw std::runtime_error("unknown component: " + std::string(name));
    return it->second;
}
} // namespace ssg::lua