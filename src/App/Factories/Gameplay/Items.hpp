#pragma once

#include <Entity.hpp>

#include "EngineContext.hpp"
#include "JsonUtil.hpp"

namespace ssg::factory
{
json::json ApplyItemDefinition(EngineContext& context, entt::registry& r, Entity entity,
                               Filepath definition);
}