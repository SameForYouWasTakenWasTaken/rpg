#pragma once

#include "Systems/ISystem.hpp"

namespace ssg
{

// Fills every entity's CWorldTransform from its CTransform (local) and its
// place in the CRelationship hierarchy. Roots copy local into world; children
// compose their local transform onto their parent's world transform.
//
// Must run once per frame BEFORE any system that reads world position
// (rendering, spatial grid, camera). Processes parents before children.
class TransformSystem final : public ISystem
{
  public:
    using ISystem::ISystem;

    void Update(float dt) override;

  private:
    struct WorldTransform; // fwd, defined in the .cpp

    void UpdateEntity(entt::entity entity, const WorldTransform& parentWorld);
};

} // namespace ssg
