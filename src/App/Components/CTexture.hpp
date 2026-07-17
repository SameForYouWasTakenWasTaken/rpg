#pragma once

#include "SFML/Graphics/Rect.hpp"
#include "Systems/AssetManager.hpp"

namespace ssg {

struct CTexture {
    TextureID textureID{0};
    sf::FloatRect textureRect{{0.0f, 0.0f}, {0.0f, 0.0f}};
};

} // namespace ssg
