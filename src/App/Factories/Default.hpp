#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <Entity.hpp>

#include "EngineContext.hpp"
#include "JsonUtil.hpp"
#include "Types.hpp"

namespace ssg::factory
{

json::json ApplyCharacterDefinition(EngineContext& context, entt::registry& r, Entity entity,
                                    Filepath definition);
} // namespace ssg::factory
