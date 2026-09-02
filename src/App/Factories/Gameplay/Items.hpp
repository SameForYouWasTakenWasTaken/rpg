#pragma once

#include <entt/entt.hpp>

#include "EngineContext.hpp"
#include "JsonUtil.hpp"

namespace ssg::factory
{
json::json ApplyItemDefinition(EngineContext& context, entt::registry& r, entt::entity entity,
                               Filepath definition);
}