#pragma once

#include <entt/entt.hpp>

#include "JsonUtil.hpp"

namespace ssg::factory
{
json::json ApplyItemDefinition(entt::registry& r, entt::entity entity, Filepath definition);
}