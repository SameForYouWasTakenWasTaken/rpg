#pragma once
#include <entt/entt.hpp>

#include "JsonUtil.hpp"
#include "Types.hpp"

namespace ssg::factory
{
json::json ApplyWeaponDefinition(entt::registry& r, entt::entity entity, Filepath definition);
}