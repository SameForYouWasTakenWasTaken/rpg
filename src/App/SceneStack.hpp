#pragma once

#include <memory>

#include "Application.hpp"
#include "Rendering/Renderer.hpp"
#include "Scene.hpp"
#include "Types.hpp"

namespace ssg
{

class SceneStack
{
  public:
    SceneStack() = default;
    ~SceneStack() = default;

    SceneStack(const SceneStack&) = delete;
    SceneStack(SceneStack&&) = delete;
    SceneStack& operator=(const SceneStack&) = delete;
    SceneStack& operator=(SceneStack&&) = delete;

    void Push(std::unique_ptr<IScene> scene);
    std::unique_ptr<IScene> Pop();

    void Switch(std::unique_ptr<IScene> scene);
    void Clear();

    bool Empty() const;

    void Update(float dt, ApplicationContext& context);
    void Render(Renderer& renderer, ApplicationContext& context);

  private:
    Vector<std::unique_ptr<IScene>> m_Scenes;
};

} // namespace ssg
