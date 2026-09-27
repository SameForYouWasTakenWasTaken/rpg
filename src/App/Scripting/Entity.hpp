#pragma once
#include <entt/entt.hpp>
#include <sol/state.hpp>

#include "LuaContext.hpp"
#include "SceneStack.hpp"

namespace ssg::lua::api::entity
{
class Entity
{
  public:
    Entity(entt::entity e, entt::registry& registry) : m_Entity(e), m_Registry(registry) {}
    void destroy();
    entt::entity get_entity();
    [[nodiscard]] bool is_valid();
    [[nodiscard]] entt::handle handle() { return {m_Registry, m_Entity}; }
    bool operator==(const Entity& other) const
    {
        return m_Entity == other.m_Entity && &m_Registry == &other.m_Registry;
    }

  private:
    entt::entity m_Entity;
    entt::registry& m_Registry;
};

void register_entity(LuaContext& lua_context);
void register_all(LuaContext& lua_context, SceneStack& sceneStack);
} // namespace ssg::lua::api::entity
