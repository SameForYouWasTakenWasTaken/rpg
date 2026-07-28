#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <entt/entt.hpp>

#include "Types.hpp"

namespace ssg::factory
{
void CreateCharacter(entt::registry& r, entt::entity entity, const Filepath& definition);
} // namespace ssg::factory
