#pragma once
#include "SFML/Graphics/Texture.hpp"
#include "Types.hpp"

namespace ssg::lua::api
{
// Wrap sf::Texture but let it contain a handle, too.
class Texture
{
  public:
    TextureHandle handle;
    const sf::Texture& texture;

    Texture(const TextureHandle handle, const sf::Texture& texture)
        : handle(handle), texture(texture)
    {
    }

    friend bool operator==(const Texture& lhs, const Texture& rhs)
    {
        return lhs.handle == rhs.handle;
    }
};
} // namespace ssg::lua::api