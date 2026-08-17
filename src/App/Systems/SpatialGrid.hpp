#pragma once

#include <cmath>
#include <cstdint>
#include <entt/entt.hpp>
#include <limits>
#include <unordered_map>

#include "Components/CTransform.hpp"
#include "Systems/ISystem.hpp"
#include "Types.hpp"

namespace ssg
{

using GridKey = std::int64_t;
using GridEntityList = Vector<entt::entity>;

class SpatialGrid : public ISystem
{
  public:
    explicit SpatialGrid(entt::registry& registry, EventBus& bus, float gridSize = 1000.0f)
        : ISystem(registry, bus), m_GridSize(gridSize)
    {
    }

    void Update(float /*dt*/) override { Rebuild(); }

    void Insert(entt::entity entity);
    void Rebuild();
    void Clear() { m_Grid.clear(); }

    template <typename TFilter>
    entt::entity FindNearest(const Vec2& pos, float radius, TFilter&& filter);

    template <typename TFilter>
    GridEntityList Query(const Vec2& pos, float radius, TFilter&& filter);

  private:
    static GridKey MakeKey(std::int32_t x, std::int32_t y)
    {
        return (static_cast<GridKey>(static_cast<std::uint32_t>(x)) << 32) |
               static_cast<std::uint32_t>(y);
    }

    template <typename TFilter, typename TCall>
    void Iterate(const Vec2& pos, float radius, TFilter&& filter, TCall&& call);

    std::unordered_map<GridKey, GridEntityList> m_Grid;

    float m_GridSize = 1000.0f;
};

template <typename TFilter, typename TCall>
void SpatialGrid::Iterate(const Vec2& pos, float radius, TFilter&& filter, TCall&& call)
{
    const auto minCellX = static_cast<std::int32_t>(std::floor((pos.x - radius) / m_GridSize));
    const auto maxCellX = static_cast<std::int32_t>(std::floor((pos.x + radius) / m_GridSize));
    const auto minCellY = static_cast<std::int32_t>(std::floor((pos.y - radius) / m_GridSize));
    const auto maxCellY = static_cast<std::int32_t>(std::floor((pos.y + radius) / m_GridSize));

    const float radiusSquared = radius * radius;

    for (std::int32_t y = minCellY; y <= maxCellY; y++)
    {
        for (std::int32_t x = minCellX; x <= maxCellX; x++)
        {
            auto it = m_Grid.find(MakeKey(x, y));
            if (it == m_Grid.end())
                continue;

            for (entt::entity entity : it->second)
            {
                if (!filter(entity))
                    continue;

                const auto& transform = m_Registry.get<CTransform>(entity);
                const Vec2 delta = transform.position - pos;

                if (glm::dot(delta, delta) <= radiusSquared)
                    call(entity);
            }
        }
    }
}

template <typename TFilter>
entt::entity SpatialGrid::FindNearest(const Vec2& pos, float radius, TFilter&& filter)
{
    entt::entity nearestEntity{entt::null};
    float bestDist = std::numeric_limits<float>::max();

    Iterate(pos, radius, filter,
            [&](entt::entity entity)
            {
                const auto& transform = m_Registry.get<CTransform>(entity);
                const Vec2 delta = transform.position - pos;
                const float dist = glm::dot(delta, delta);

                if (dist < bestDist)
                {
                    bestDist = dist;
                    nearestEntity = entity;
                }
            });

    return nearestEntity;
}

template <typename TFilter>
GridEntityList SpatialGrid::Query(const Vec2& pos, float radius, TFilter&& filter)
{
    GridEntityList neighbours;

    Iterate(pos, radius, filter, [&](entt::entity entity) { neighbours.push_back(entity); });

    return neighbours;
}

} // namespace ssg
