#include "GameScene.hpp"

#include "Application.hpp"

namespace ssg
{

GameScene::~GameScene()
{
    for (auto& layer : m_Layers)
    {
        layer->OnDetach(m_SceneContext);
    }
}

void GameScene::OnUpdate(float dt)
{
    for (auto& layer : m_Layers)
    {
        layer->OnUpdate(dt, m_SceneContext);
    }
}

void GameScene::OnRender()
{
    for (auto& layer : m_Layers)
    {
        layer->OnRender(m_SceneContext);
    }
}

} // namespace ssg
