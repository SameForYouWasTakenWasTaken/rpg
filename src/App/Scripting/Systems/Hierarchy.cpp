#include "Hierarchy.hpp"

#include "Entity.hpp"
#include "Scripting/Entity.hpp"
#include "Systems/Hierarchy.hpp"

namespace ssg::lua::api::hierarchy
{

void helper_attach(LuaContext& context, entt::registry& r, Entity parent, Entity child)
{
    using namespace ssg::hierarchy;
    AttachChild(r, parent, child, AttachMode::KeepWorld);
}
void helper_detach(LuaContext& context, entt::registry& r, Entity child)
{
    using namespace ssg::hierarchy;
    DetachChild(r, child);
}
void register_all(LuaContext& context, SceneStack& stack)
{
    auto engine_table = context.namespace_table;
    auto hierarchyTable = engine_table.create_named("hierarchy");
    hierarchyTable.set_function("attach_child",
                                [&context, &stack](Entity parent, Entity child)
                                {
                                    if (entity::valid_current_scene(stack))
                                        helper_attach(context, entity::current_registry(stack),
                                                      parent, child);
                                });

    hierarchyTable.set_function("detach_child",
                                [&context, &stack](Entity child)
                                {
                                    if (entity::valid_current_scene(stack))
                                        helper_detach(context, entity::current_registry(stack),
                                                      child);
                                });

    hierarchyTable.set_function("is_root",
                                [&stack](Entity e)
                                {
                                    return entity::valid_current_scene(stack) &&
                                           ssg::hierarchy::IsRoot(entity::current_registry(stack),
                                                                  e);
                                });

    hierarchyTable.set_function("get_root",
                                [&stack](Entity e) -> sol::optional<Entity>
                                {
                                    if (!entity::valid_current_scene(stack))
                                        return sol::nullopt;
                                    auto root =
                                        ssg::hierarchy::GetRoot(entity::current_registry(stack), e);
                                    if (root == entt::null)
                                        return sol::nullopt;
                                    return root;
                                });
}
} // namespace ssg::lua::api::hierarchy