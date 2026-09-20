#pragma once

#include "Application.hpp"
#include "EngineContext.hpp"
#include "Layers/GameLayer.hpp"
#include "Rendering/Renderer.hpp"
#include "Scene.hpp"
#include "Systems/Gameplay/Combat.hpp"
#include "Systems/SpatialGrid.hpp"
#include "Systems/SystemRegistry.hpp"
#include "Systems/TransformSystem.hpp"

namespace ssg
{

class GameScene final : public IScene
{
  public:
    GameScene(EngineContext& engine_context, ApplicationContext& application_context)
        : IScene(engine_context, application_context)
    {

        auto& spatialGrid = m_SystemRegistry.Register(
            std::move(std::make_unique<SpatialGrid>(m_Registry, m_SceneContext.engine_context)));

        auto& transformSystem = m_SystemRegistry.Register(std::move(
            std::make_unique<TransformSystem>(m_Registry, m_SceneContext.engine_context)));

        auto& combatSystem = m_SystemRegistry.Register(std::move(std::make_unique<CombatSystem>(
            m_Registry, m_SceneContext.engine_context, spatialGrid)));
    }
    ~GameScene();

    GameScene(const GameScene&) = delete;
    GameScene(GameScene&&) = delete;
    GameScene& operator=(const GameScene&) = delete;
    GameScene& operator=(GameScene&&) = delete;

    void OnUpdate(float dt) override;
    void OnRender() override;
};

} // namespace ssg
