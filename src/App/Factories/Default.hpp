#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <entt/entt.hpp>

#include "JsonUtil.hpp"
#include "Types.hpp"

namespace ssg::factory
{
json::json ApplyCharacterDefinition(entt::registry& r, entt::entity entity, Filepath definition);
} // namespace ssg::factory
