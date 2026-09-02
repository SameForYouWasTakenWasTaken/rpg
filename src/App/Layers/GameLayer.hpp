#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <memory>

#include "EngineContext.hpp"
#include "Events/KeyPressedEvent.hpp"
#include "Events/WindowResizeEvent.hpp"
#include "ILayer.hpp"
#include "Rendering/Camera.hpp"
#include "Rendering/Renderer.hpp"
#include "Systems/Gameplay/Combat.hpp"
#include "Systems/SpatialGrid.hpp"
#include "Systems/TransformSystem.hpp"
#include "Types.hpp"

namespace ssg
{

class GameLayer final : public ILayer
{
  public:
    GameLayer(EngineContext& context) : m_EngineContext(context) {}
    ~GameLayer() override = default;

    GameLayer(const GameLayer&) = delete;
    GameLayer(GameLayer&&) = delete;
    GameLayer& operator=(const GameLayer&) = delete;
    GameLayer& operator=(GameLayer&&) = delete;

    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate(float dt, ApplicationContext& context) override;
    void OnRender(Renderer& renderer, ApplicationContext& context) override;

  private:
    void OnWindowResize(const WindowResizeEvent& event);
    void OnKeyPress(const KeyPressedEvent& event);

    EngineContext& m_EngineContext;
    entt::entity m_LocalPlayer{entt::null};
    Camera m_LocalPlayerCamera;
    entt::registry m_Registry;

    SpatialGrid m_SpatialGrid{m_Registry, m_EngineContext};
    TransformSystem m_TransformSystem{m_Registry, m_EngineContext};
    CombatSystem m_CombatSystem{m_Registry, m_EngineContext, m_SpatialGrid};
};

} // namespace ssg
