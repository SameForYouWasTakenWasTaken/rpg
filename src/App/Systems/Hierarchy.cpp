#include "Hierarchy.hpp"

#include "Components/CRelationship.hpp"
#include "Components/CTransform.hpp"
#include "Components/CWorldTransform.hpp"

namespace ssg::hierarchy
{
void AttachChild(entt::registry& registry, entt::entity parent, entt::entity child)
{
    auto& parentRelation = registry.get_or_emplace<CRelationship>(parent);
    auto& childRelation = registry.get_or_emplace<CRelationship>(child);
    // The new child becomes the head of the parent's child list.
    childRelation.parent = parent;
    childRelation.prev = entt::null;           // head has no previous sibling
    childRelation.next = parentRelation.first; // link to the old first child
    // The old first child now sits after the new child, so fix its back-link.
    if (parentRelation.first != entt::null)
    {
        auto& oldFirstChild = registry.get<CRelationship>(parentRelation.first);
        oldFirstChild.prev = child;
    }
    parentRelation.first = child; // the new child is now the head
    parentRelation.children++;
}

void AttachChild(entt::registry& registry, entt::entity parent, entt::entity child,
                 AttachMode mode)
{
    AttachChild(registry, parent, child);

    if (mode == AttachMode::KeepWorld)
    {
        // Rebase the child's local transform so its world position is unchanged.
        // No-op if either entity lacks the transforms this needs.
        auto* childLocal = registry.try_get<CTransform>(child);
        const auto* childWorld = registry.try_get<CWorldTransform>(child);
        const auto* parentWorld = registry.try_get<CWorldTransform>(parent);

        if (childLocal != nullptr && childWorld != nullptr && parentWorld != nullptr)
        {
            childLocal->position = (childWorld->position - parentWorld->position) / parentWorld->scale;
            childLocal->scale = childWorld->scale / parentWorld->scale;
            childLocal->rotation = childWorld->rotation - parentWorld->rotation;
        }
    }
}

void DetachChild(entt::registry& registry, entt::entity child)
{
    auto& childRelation = registry.get<CRelationship>(child);

    if (childRelation.parent == entt::null)
        return; // already a root, nothing to unlink

    auto& parentRelation = registry.get<CRelationship>(childRelation.parent);
    // Unlink from the previous sibling (or from the parent if we were the head).
    if (childRelation.prev != entt::null)
    {
        auto& previousSibling = registry.get<CRelationship>(childRelation.prev);
        previousSibling.next = childRelation.next;
    }
    else
    {
        parentRelation.first = childRelation.next; // child was the head
    }
    // Unlink from the following sibling.
    if (childRelation.next != entt::null)
    {
        auto& nextSibling = registry.get<CRelationship>(childRelation.next);
        nextSibling.prev = childRelation.prev;
    }

    parentRelation.children--;
    // The child is now a root again.
    childRelation.parent = entt::null;
    childRelation.prev = entt::null;
    childRelation.next = entt::null;
}
} // namespace ssg::hierarchy