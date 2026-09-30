#pragma once
#include <Entity.hpp>
#include <sol/state.hpp>

namespace ssg::lua
{
class ScriptEntity
{

  public:
    Entity handle;
    entt::registry* registry;

    static void Register(sol::state& state);

    void Destroy()
    {
        if (IsValid())
        {
            registry->destroy(handle);
            handle = entt::null;
        }
    }

    [[nodiscard]] bool IsValid() const { return registry && registry->valid(handle); }

  private:
};
} // namespace ssg::lua
