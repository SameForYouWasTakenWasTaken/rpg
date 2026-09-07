#include "Tilemap.hpp"

namespace ssg::map
{

const Tileset* Tilemap::getTilesetForGid(TileLayer::TileID gid) const
{
    for (auto it = m_Sets.rbegin(); it != m_Sets.rend(); ++it)
    {
        if (gid >= it->getFirstGid())
            return &*it;
    }

    return nullptr;
}
} // namespace ssg::map