#pragma once

#include <entt/entt.hpp>

#include "EngineContext.hpp"
#include "Events/Gameplay/OnAttackRequest.hpp"
#include "Systems/ISystem.hpp"
#include "Systems/SpatialGrid.hpp"

namespace ssg
{
class CombatSystem : public ISystem
{
  public:
    CombatSystem(entt::registry& registry, EngineContext& context, SpatialGrid& spatial_grid);

    CombatSystem(const CombatSystem& other) = delete;
    CombatSystem(CombatSystem&& other) noexcept = delete;
    CombatSystem& operator=(const CombatSystem& other) = delete;
    CombatSystem& operator=(CombatSystem&& other) noexcept = delete;

    void Update(float /*dt*/) override;

  private:
    void OnAttackEvent(OnAttackRequest& event);
    void ResolveAttack(entt::entity attacker, entt::entity defender) const;

    SpatialGrid& m_SpatialGrid;
};
} // namespace ssg