#pragma once

#include "Application.hpp"
#include "Layers/GameLayer.hpp"
#include "Rendering/Renderer.hpp"
#include "Scene.hpp"

namespace ssg
{

class GameScene final : public IScene
{
  public:
    GameScene();
    ~GameScene();

    GameScene(const GameScene&) = delete;
    GameScene(GameScene&&) = delete;
    GameScene& operator=(const GameScene&) = delete;
    GameScene& operator=(GameScene&&) = delete;

    void OnUpdate(float dt, ApplicationContext& context) override;
    void OnRender(Renderer& renderer, ApplicationContext& context) override;
};

} // namespace ssg
