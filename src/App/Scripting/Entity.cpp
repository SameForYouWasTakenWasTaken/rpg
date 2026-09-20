#include "Entity.hpp"

#include <entt/entt.hpp>

#include "ComponentView.hpp"
#include "SceneStack.hpp"
#include "Types/ScriptEntity.hpp"

namespace ssg::lua::api
{
void entity::Entity::destroy()
{
    if (is_valid())
    {
        m_Registry.destroy(m_Entity);
        m_Entity = entt::null;
    }
}
entt::entity entity::Entity::get_entity() { return is_valid() ? m_Entity : entt::null; }

bool entity::Entity::is_valid() { return m_Entity != entt::null && m_Registry.valid(m_Entity); }
void entity::register_entity(sol::table& engine)
{
    engine.new_usertype<Entity>(
        "Entity", sol::no_constructor, "valid", &Entity::is_valid, "destroy", &Entity::destroy,

        "add",
        [](Entity& e, const std::string& name, sol::optional<sol::table> args, sol::this_state ts)
        {
            sol::table t = args ? *args : sol::state_view(ts).create_table();
            find_ops(name).add(e.handle(), t);
        },
        "has", [](Entity& e, const std::string& name) { return find_ops(name).has(e.handle()); },
        "remove", [](Entity& e, const std::string& name) { find_ops(name).remove(e.handle()); },
        "get", [](Entity& e, const std::string& name, sol::this_state ts)
        { return find_ops(name).get(e.handle(), ts); },

        sol::meta_function::equal_to, &Entity::operator==);
}

void entity::register_all(sol::table& table, SceneStack& sceneStack)
{
    sol::table entity_table = table.create_named("entity");

    register_entity(table);
    entity_table.set_function("create",
                              [&sceneStack]
                              {
                                  entt::registry& reg = sceneStack.Current()->GetContext().registry;
                                  return Entity{reg.create(), reg};
                              });
}
} // namespace ssg::lua::api