#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <entt/entt.hpp>

#include "Types.hpp"

namespace ssg::factory
{

// Adds a local + world transform and a hierarchy node (as a root) to an
// existing entity. All values are defaulted: position {0,0}, scale {1,1},
// rotation 0, no parent.
void AddDefaultTransform(entt::registry& registry, entt::entity entity);

// Adds a default (empty) texture to an existing entity: textureID 0, empty
// rect. Also adds a transform and hierarchy node so it is ready to place/parent.
void AddDefaultTexture(entt::registry& registry, entt::entity entity);

// Adds sprite components to an existing entity. A sprite needs a texture, so
// the texture id and source rect are required. Adds transform, world transform,
// texture, sprite and a hierarchy node. The transform scale stays a multiplier
// ({1,1}); `size` is the sprite's pixel size.
void AddDefaultSprite(entt::registry& registry, entt::entity entity, TextureID texID,
                      const sf::FloatRect& texRect, Vec2 size = {100.0f, 100.0f});

} // namespace ssg::factory
