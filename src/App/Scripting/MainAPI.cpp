#include "MainAPI.hpp"

#include "ComponentView.hpp"
#include "Entity.hpp"
#include "Logger.hpp"
#include "Systems/AssetManager.hpp"
#include "Types/FloatRect.hpp"
#include "Types/SSGTypes.hpp"

namespace ssg::lua::api
{

void lua_api_init(sol::state& state, EngineContext& context, SceneStack& stack)
{
    sol::table engine_table = state.create_named_table("engine");
    register_print(context, state);
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
void register_print(EngineContext& context, sol::state& state)
{
    auto& logger = context.logger;
    state.set_function("print",
                       [&logger](sol::this_state ts, sol::variadic_args args)
                       {
                           lua_State* L = ts;
                           std::string line;
                           bool first = true;

                           for (auto arg : args)
                           {
                               if (!first)
                                   line += '\t';
                               first = false;

                               size_t len = 0;
                               const char* s = luaL_tolstring(L, arg.stack_index(), &len);
                               line.append(s, len);
                               lua_pop(L, 1); // luaL_tolstring pushes its result
                           }

                           ScriptLocation loc = current_script_location(L);
                           logger.Info(log::LogLocation(loc.file, static_cast<uint32_t>(loc.line)),
                                       "lua", "{}", line);
                       });
}
} // namespace ssg::lua::api