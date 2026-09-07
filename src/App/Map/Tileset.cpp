#include "Tileset.hpp"

namespace ssg::map
{
Region Tileset::getRegion(std::uint32_t localId) const
{
    const auto column = localId % m_Columns;
    const auto row = localId / m_Columns;

    return Region{
        {static_cast<float>(column * m_tileWidth), static_cast<float>(row * m_tileHeight)},
        {static_cast<float>(m_tileWidth), static_cast<float>(m_tileHeight)},
    };
}
} // namespace ssg::map