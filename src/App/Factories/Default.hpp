#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <entt/entt.hpp>

#include "Types.hpp"

namespace ssg::factory
{
void ApplyCharacterDefinition(entt::registry& r, entt::entity entity, Filepath definition);
} // namespace ssg::factory
