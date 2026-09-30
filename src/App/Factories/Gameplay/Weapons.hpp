#pragma once
#include <Entity.hpp>

#include "EngineContext.hpp"
#include "JsonUtil.hpp"
#include "Types.hpp"

namespace ssg::factory
{
json::json ApplyWeaponDefinition(EngineContext& context, entt::registry& r, Entity entity,
                                 Filepath definition);
}