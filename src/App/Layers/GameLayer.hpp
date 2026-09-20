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
#include "Rendering/SpriteSink.hpp"
#include "Systems/Gameplay/Combat.hpp"
#include "Systems/SpatialGrid.hpp"
#include "Systems/TransformSystem.hpp"
#include "Types.hpp"

namespace ssg
{

class GameLayer final : public ILayer
{
  public:
    GameLayer() = default;
    ~GameLayer() override = default;

    GameLayer(const GameLayer&) = delete;
    GameLayer(GameLayer&&) = delete;
    GameLayer& operator=(const GameLayer&) = delete;
    GameLayer& operator=(GameLayer&&) = delete;

    void OnAttach(context::SceneContext& context) override;
    void OnDetach(context::SceneContext& context) override;

    void OnUpdate(float dt, context::SceneContext& context) override;
    void OnRender(context::SceneContext& context) override;

  private:
    void OnWindowResize(const WindowResizeEvent& event);
    void OnKeyPress(const KeyPressedEvent& event);

    rendering::SpriteSink* m_SpriteSink;

    entt::entity m_LocalPlayer{entt::null};
    Camera m_LocalPlayerCamera;

    TransformSystem* m_TransformSystem = nullptr;
    SpatialGrid* m_SpatialGrid = nullptr;
    CombatSystem* m_CombatSystem = nullptr;
    EngineContext* m_EngineContext = nullptr;
    entt::registry* m_Registry = nullptr;
};

} // namespace ssg
