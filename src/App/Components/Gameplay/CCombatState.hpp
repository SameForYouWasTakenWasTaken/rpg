#pragma once

#include <entt/entt.hpp>
#include <vector>

namespace ssg
{
struct CCombatState
{
    bool isAttacking = false;
    float attackTimer = 0.f;    // counts down, time left in swing
    float timeInAttack = 0.f;   // counts up, elapsed since swing start
    float swingDuration = 0.0f; // cached = 1 / attackSpeed at start
    float windowLo = 0.0f;      // cached absolute open time
    float windowHi = 0.0f;      // cached absolute close time

    entt::entity cachedWeapon{entt::null};
    std::vector<entt::entity> alreadyHit{};
};
} // namespace ssg