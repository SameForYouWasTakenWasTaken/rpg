#include "Default.hpp"

#include "Components/CRelationship.hpp"
#include "Components/CSprite.hpp"
#include "Components/CTexture.hpp"
#include "Components/CTransform.hpp"
#include "Components/CWorldTransform.hpp"

namespace ssg::factory
{

void AddDefaultTransform(entt::registry& registry, entt::entity entity)
{
    registry.emplace<CTransform>(entity);
    registry.emplace<CWorldTransform>(entity);
    registry.emplace<CRelationship>(entity); // root of the hierarchy
}

void AddDefaultTexture(entt::registry& registry, entt::entity entity)
{
    AddDefaultTransform(registry, entity);

    registry.emplace<CTexture>(entity); // textureID 0, empty rect
}

void AddDefaultSprite(entt::registry& registry, entt::entity entity, TextureID texID,
                      const sf::FloatRect& texRect, Vec2 size)
{
    AddDefaultTransform(registry, entity);

    auto& sprite = registry.emplace<CSprite>(entity);
    sprite.origin = {0.5f, 0.5f};
    sprite.size = size;

    auto& texture = registry.emplace<CTexture>(entity);
    texture.textureID = texID;
    texture.textureRect = texRect;
}

} // namespace ssg::factory
