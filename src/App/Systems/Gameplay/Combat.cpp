#include "Combat.hpp"

#include "Components/CWorldTransform.hpp"
#include "Components/Gameplay/CCombatState.hpp"
#include "Components/Gameplay/CEquipment.hpp"
#include "Components/Gameplay/CHealth.hpp"
#include "Components/Gameplay/CHumanoid.hpp"
#include "Components/Gameplay/CWeapon.hpp"
#include "Engine.hpp"
#include "Events/Gameplay/OnEntityDeath.hpp"

namespace ssg
{
CombatSystem::CombatSystem(entt::registry& registry, EngineContext& engineContext,
                           SpatialGrid& grid)
    : ISystem(registry, engineContext), m_SpatialGrid(grid)
{
    engineContext.engine.GetEventBus()
        .Sink<OnAttackRequest>()
        .connect<&CombatSystem::OnAttackEvent>(this);
}

void CombatSystem::ResolveAttack(entt::entity attacker, entt::entity target) const
{
    float damage =
        m_Registry.get<CWeapon>(m_Registry.get<CCombatState>(attacker).cachedWeapon).damage;
    auto& hp = m_Registry.get<CHealth>(target);
    hp.current -= damage;
    m_EngineContext.logger.Info("Combat", "Hit enemy {} for {}!",
                                static_cast<std::uint32_t>(target), damage);
    if (hp.current <= 0.0f)
        m_EngineContext.engine.GetEventBus().Queue<OnEntityDeath>(target);
}
void CombatSystem::Update(float dt)
{
    auto view = m_Registry.view<CCombatState, CEquipment, CWorldTransform>();
    for (auto attacker : view)
    {
        auto& combatState = view.get<CCombatState>(attacker);
        if (!combatState.isAttacking)
            continue;

        // timers
        combatState.attackTimer -= dt;
        combatState.timeInAttack += dt;

        // Hit window check
        if (combatState.timeInAttack >= combatState.windowLo &&
            combatState.timeInAttack <= combatState.windowHi)
        {
            auto& transform = m_Registry.get<CWorldTransform>(attacker);
            float range = m_Registry.get<CWeapon>(combatState.cachedWeapon).range;

            // query target entities within range of entity, that are a humanoid and alive
            for (entt::entity target : m_SpatialGrid.Query(
                     transform.position, range,
                     [attacker, this](entt::entity target)
                     {
                         return target != attacker && m_Registry.all_of<CHealth, CHumanoid>(target);
                     }))
            {
                // skip already cached entities
                if (std::ranges::contains(combatState.alreadyHit, target))
                    continue;

                ResolveAttack(attacker, target);
                combatState.alreadyHit.push_back(target);
            }
        }

        if (combatState.attackTimer <= 0.0f)
        { // ALWAYS reached now
            combatState.isAttacking = false;
            combatState.timeInAttack = 0.0f;
            combatState.attackTimer = 0.0f;
            combatState.alreadyHit.clear();
        }
    }
}

void CombatSystem::OnAttackEvent(OnAttackRequest& event)
{
    auto& combatState = m_Registry.get<CCombatState>(event.Attacker);

    if (combatState.isAttacking || combatState.attackTimer > 0.0f)
        return;

    auto& equipment = m_Registry.get<CEquipment>(event.Attacker);
    if (equipment.weapon == entt::null)
        return; // unarmed guard

    auto& weapon = m_Registry.get<CWeapon>(equipment.weapon);
    float swing =
        (weapon.attackSpeed > 0.0f) ? 1.0f / weapon.attackSpeed : 1.0f; // div-by-zero guard

    combatState.isAttacking = true;
    combatState.attackTimer = swing;
    combatState.timeInAttack = 0.0f;
    combatState.swingDuration = swing;
    combatState.windowLo = weapon.hitWindow.x * swing;
    combatState.windowHi = weapon.hitWindow.y * swing;
    combatState.cachedWeapon = equipment.weapon;
    combatState.alreadyHit.clear();
}

} // namespace ssg