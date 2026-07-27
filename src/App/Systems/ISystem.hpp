#pragma once

#include <entt/entt.hpp>

namespace ssg
{

// Base class for systems that operate on a scene's entity registry.
//
// A system holds a reference to the registry it works on and does its work in
// Update(), called once per frame by the owning layer/scene. Systems are the
// only place that should mutate the components they own (see the transform
// pipeline: local -> world).
class ISystem
{
  public:
    explicit ISystem(entt::registry& registry) : m_Registry(registry) {}
    virtual ~ISystem() = default;

    ISystem(const ISystem&) = delete;
    ISystem(ISystem&&) = delete;
    ISystem& operator=(const ISystem&) = delete;
    ISystem& operator=(ISystem&&) = delete;

    virtual void Update(float dt) = 0;

  protected:
    entt::registry& m_Registry;
};

} // namespace ssg
