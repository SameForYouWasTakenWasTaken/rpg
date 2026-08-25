#include "Systems/TransformSystem.hpp"

#include "Components/CRelationship.hpp"
#include "Components/CTransform.hpp"
#include "Components/CWorldTransform.hpp"

namespace ssg
{

// Local, mutable copy of a world transform used while descending the tree.
struct TransformSystem::WorldTransform
{
    Vec2 position{0.0f, 0.0f};
    Vec2 scale{1.0f, 1.0f};
    float rotation{0.0f};
};

void TransformSystem::Update(float /*dt*/)
{
    static const WorldTransform identity{};

    // Start from every root: an entity that has no relationship, or whose
    // relationship has no parent. Children are reached recursively from there.
    for (auto entity : m_Registry.view<CTransform>())
    {
        const auto* relation = m_Registry.try_get<CRelationship>(entity);
        const bool isRoot = (relation == nullptr) || (relation->parent == entt::null);

        if (isRoot)
            UpdateEntity(entity, identity);
    }
}

void TransformSystem::UpdateEntity(entt::entity entity, const WorldTransform& parentWorld)
{
    if (entity == entt::null)
        return;
    const auto& local = m_Registry.get<CTransform>(entity);

    // Compose local onto the parent's world transform.
    WorldTransform world;
    world.scale = parentWorld.scale * local.scale;
    world.rotation = parentWorld.rotation + local.rotation;
    world.position = parentWorld.position + parentWorld.scale * local.position;

    auto& worldComponent = m_Registry.get_or_emplace<CWorldTransform>(entity);
    worldComponent.position = world.position;
    worldComponent.scale = world.scale;
    worldComponent.rotation = world.rotation;

    // Recurse into children (parent-first ordering is guaranteed this way).
    const auto* relation = m_Registry.try_get<CRelationship>(entity);
    if (relation == nullptr)
        return;

    entt::entity child = relation->first;
    while (child != entt::null)
    {
        UpdateEntity(child, world);
        child = m_Registry.get<CRelationship>(child).next;
    }
}

} // namespace ssg
