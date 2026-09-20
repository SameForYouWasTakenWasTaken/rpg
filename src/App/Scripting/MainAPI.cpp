#include "MainAPI.hpp"

#include "ComponentView.hpp"
#include "Entity.hpp"
#include "Systems/AssetManager.hpp"
#include "Types/FloatRect.hpp"
#include "Types/SSGTypes.hpp"

namespace ssg::lua::api
{

void lua_api_init(sol::state& state, EngineContext& context, SceneStack& stack)
{
    sol::table engine_table = state.create_named_table("engine");
    glob_types::register_all(state);
    register_component_refs(state, ScriptComponents{});
    register_sf_float_rect(state);
    assets::register_all(engine_table, context);
    entity::register_all(engine_table, stack);
}
void update(sol::state& state, float dt)
{
    const sol::function _update = state["_update"];
    _update.call<void>(dt);
}
} // namespace ssg::lua::api