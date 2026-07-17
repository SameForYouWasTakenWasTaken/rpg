#pragma once

#include "SFML/Graphics/Color.hpp"
#include "Types.hpp"

namespace ssg {

struct CSprite {
    sf::Color color{sf::Color::White};
    Vec2 origin{0.0f, 0.0f}; // Normalized, 0 - 1
    bool flipX{false};
    bool flipY{false};
};

} // namespace ssg
