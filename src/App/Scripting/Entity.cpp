#include "Entity.hpp"

#include <Entity.hpp>

#include "Components/ComponentView.hpp"
#include "SceneStack.hpp"

namespace ssg::lua::api::entity
{
entt::registry& current_registry(SceneStack& sceneStack)
{
    return sceneStack.Current()->GetContext().registry;
}

// Throws a Lua error instead of hitting an entt assert on stale ids
entt::handle handle_of(SceneStack& sceneStack, Entity e)
{
    entt::registry& reg = current_registry(sceneStack);
    if (!reg.valid(e.id))
        throw std::runtime_error("invalid entity");
    return entt::handle{reg, e.id};
}

void register_entity(LuaContext& lua_context, SceneStack& sceneStack)
{
    auto& engine = lua_context.namespace_table;
    auto* stack = &sceneStack;

    engine.new_usertype<Entity>(
        "Entity", sol::no_constructor,

        "valid", [stack](Entity e) { return current_registry(*stack).valid(e); },

        "destroy",
        [stack](Entity e)
        {
            if (auto& reg = current_registry(*stack); reg.valid(e))
                reg.destroy(e);
        },

        "add",
        [stack](Entity e, const std::string& name, sol::optional<sol::table> args,
                sol::this_state ts)
        {
            sol::table t = args ? *args : sol::state_view(ts).create_table();
            find_ops(name).add(handle_of(*stack, e), t);
        },

        "has",
        [stack](Entity e, const std::string& name)
        {
            entt::registry& reg = current_registry(*stack);
            return reg.valid(e) && find_ops(name).has(entt::handle{reg, e});
        },

        "remove",
        [stack](Entity e, const std::string& name) { find_ops(name).remove(handle_of(*stack, e)); },

        "get", [stack](Entity e, const std::string& name, sol::this_state ts)
        { return find_ops(name).get(handle_of(*stack, e), ts); },

        sol::meta_function::equal_to,
        [stack](const Entity& a, const Entity& b)
        {
            return a.id == b.id &&
                   current_registry(*stack).valid(a.id) == current_registry(*stack).valid(b.id);
        });
}

void register_all(LuaContext& lua_context, SceneStack& sceneStack)
{
    auto& table = lua_context.namespace_table;
    sol::table entity_table = table.create_named("entity");

    register_entity(lua_context, sceneStack);

    entity_table.set_function("create", [&sceneStack]()
                              { return Entity{current_registry(sceneStack).create()}; });

    entity_table.set_function("get_all_of",
                              [&sceneStack](sol::this_state ts, sol::variadic_args va)
                              {
                                  std::vector<std::string_view> names;
                                  names.reserve(va.size());
                                  for (auto arg : va)
                                      names.push_back(arg.as<std::string_view>());

                                  return get_all_of(current_registry(sceneStack),
                                                    sol::state_view(ts), names);
                              });
}
} // namespace ssg::lua::api::entity