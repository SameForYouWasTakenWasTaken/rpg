#include "TileLayer.hpp"

namespace ssg::map
{

TileLayer::TileID TileLayer::at(std::uint32_t x, std::uint32_t y) const
{
    return getTiles()[y * m_Width + x];
}
} // namespace ssg::map