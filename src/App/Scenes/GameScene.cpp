#include "GameScene.hpp"

#include "Application.hpp"

namespace ssg
{

GameScene::GameScene() {}

GameScene::~GameScene()
{
    for (auto& layer : m_Layers)
    {
        layer->OnDetach();
    }
}

void GameScene::OnUpdate(float dt, ApplicationContext& context)
{
    for (auto& layer : m_Layers)
    {
        layer->OnUpdate(dt, context);
    }
}

void GameScene::OnRender(Renderer& renderer, ApplicationContext& context)
{
    for (auto& layer : m_Layers)
    {
        layer->OnRender(renderer, context);
    }
}

} // namespace ssg
