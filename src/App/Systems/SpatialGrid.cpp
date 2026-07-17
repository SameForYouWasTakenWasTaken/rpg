#include "Systems/SpatialGrid.hpp"

namespace ssg
{

void SpatialGrid::Insert(entt::entity entity)
{
    const auto& transform = m_Registry.get<CTransform>(entity);

    const auto cellX = static_cast<std::int32_t>(std::floor(transform.position.x / m_GridSize));
    const auto cellY = static_cast<std::int32_t>(std::floor(transform.position.y / m_GridSize));

    m_Grid[MakeKey(cellX, cellY)].push_back(entity);
}

void SpatialGrid::Rebuild()
{
    m_Grid.clear();

    for (entt::entity entity : m_Registry.view<CTransform>())
        Insert(entity);
}

} // namespace ssg
