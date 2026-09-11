#pragma once

#include "SFML/Graphics/Rect.hpp"
#include "Types.hpp"

namespace ssg
{

struct CTexture
{
    TextureHandle textureID{0};
    sf::FloatRect textureRect{{0.0f, 0.0f}, {0.0f, 0.0f}};
};

} // namespace ssg
