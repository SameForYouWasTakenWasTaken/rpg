#pragma once

#include <entt/entt.hpp>
#include <memory>

#include "Application.hpp"
#include "ILayer.hpp"
#include "Rendering/Renderer.hpp"
#include "Types.hpp"

namespace ssg
{

class IScene
{
  public:
    IScene(EngineContext& engine_context, ApplicationContext& application_context)
        : m_SceneContext(m_SystemRegistry, engine_context, application_context, m_Registry)
    {
    }
    virtual ~IScene() = default;

    IScene(const IScene&) = delete;
    IScene(IScene&&) = delete;
    IScene& operator=(const IScene&) = delete;
    IScene& operator=(IScene&&) = delete;

    virtual void OnUpdate(float dt) = 0;
    virtual void OnRender() = 0;

    void PushLayer(std::unique_ptr<ILayer> layer);
    std::unique_ptr<ILayer> PopLayer(std::unique_ptr<ILayer> layer);
    context::SceneContext& GetContext() { return m_SceneContext; }

  protected:
    SystemRegistry m_SystemRegistry;
    context::SceneContext m_SceneContext;
    entt::registry m_Registry;
    Vector<std::unique_ptr<ILayer>> m_Layers = {};
};

} // namespace ssg
