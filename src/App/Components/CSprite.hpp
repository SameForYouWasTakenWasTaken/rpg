#pragma once

#include "SFML/Graphics/Color.hpp"
#include "Types.hpp"

namespace ssg
{

struct CSprite
{
    sf::Color color{sf::Color::White};
    zIndex_t zIndex{0};

    Vec2 origin{0.0f, 0.0f}; // Normalized, 0 - 1

    Vec2 size{100.0f, 100.0f}; // Pixel size before the transform scale multiplier

    bool flipX{false}; // TODO: make work
    bool flipY{false}; // TODO: make work
};

} // namespace ssg
