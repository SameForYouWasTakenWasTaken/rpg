#pragma once
#include <entt/entt.hpp>

#include "EngineContext.hpp"
#include "JsonUtil.hpp"
#include "Types.hpp"

namespace ssg::factory
{
json::json ApplyWeaponDefinition(EngineContext& context, entt::registry& r, entt::entity entity,
                                 Filepath definition);
}