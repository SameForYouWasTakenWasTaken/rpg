#include "Hierarchy.hpp"

#include "Entity.hpp"
#include "Systems/Hierarchy.hpp"

namespace ssg::lua::api::hierarchy
{

void register_attach(LuaContext& context, entt::registry& r, Entity parent, Entity child)
{
    using namespace ssg::hierarchy;
    AttachChild(r, parent, child, AttachMode::KeepWorld);
}
void register_all(LuaContext& context, SceneStack& stack, sol::table namespaceTable)
{
    auto hierarchyTable = namespaceTable.create_named("hierarchy");
    hierarchyTable.set_function("attach_child", [&context]() { register_attach(context); });
}
} // namespace ssg::lua::api::hierarchy