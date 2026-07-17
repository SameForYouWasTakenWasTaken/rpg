#include "SceneStack.hpp"

#include "Application.hpp"

namespace ssg
{

void SceneStack::Push(std::unique_ptr<IScene> scene) { m_Scenes.push_back(std::move(scene)); }

std::unique_ptr<IScene> SceneStack::Pop()
{
    if (Empty())
    {
        return nullptr;
    }

    std::unique_ptr<IScene> top = std::move(m_Scenes.back());
    m_Scenes.pop_back();
    return top;
}

void SceneStack::Switch(std::unique_ptr<IScene> scene)
{
    Pop();
    Push(std::move(scene));
}

void SceneStack::Clear() { m_Scenes.clear(); }

bool SceneStack::Empty() const { return m_Scenes.empty(); }

void SceneStack::Update(float dt, ApplicationContext& context)
{
    if (!m_Scenes.empty())
    {
        m_Scenes.back()->OnUpdate(dt, context);
    }
}

void SceneStack::Render(Renderer& renderer, ApplicationContext& context)
{
    if (!m_Scenes.empty())
    {
        m_Scenes.back()->OnRender(renderer, context);
    }
}

} // namespace ssg
