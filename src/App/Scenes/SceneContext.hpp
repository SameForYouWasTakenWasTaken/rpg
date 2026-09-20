#pragma once
#include <entt/entity/registry.hpp>

#include "Systems/SystemRegistry.hpp"

namespace ssg::context
{
struct SceneContext
{
    SystemRegistry& systemRegistry;

    EngineContext& engine_context;
    ApplicationContext& application_context;

    entt::registry& registry;

    SceneContext(SystemRegistry& system_registry, EngineContext& engine_context,
                 ApplicationContext& application_context, entt::registry& registry)
        : systemRegistry(system_registry), engine_context(engine_context),
          application_context(application_context), registry(registry)
    {
    }
};
} // namespace ssg::context