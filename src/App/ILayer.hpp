#pragma once

#include "Application.hpp"
#include "Rendering/Renderer.hpp"
#include "Scenes/SceneContext.hpp"

namespace ssg
{
class ILayer
{
  public:
    ILayer() = default;
    virtual ~ILayer() = default;

    ILayer(const ILayer&) = delete;
    ILayer(ILayer&&) = delete;
    ILayer& operator=(const ILayer&) = delete;
    ILayer& operator=(ILayer&&) = delete;

    virtual void OnAttach(context::SceneContext& context) = 0;
    virtual void OnDetach(context::SceneContext& context) = 0;

    virtual void OnUpdate(float dt, context::SceneContext& context) = 0;
    virtual void OnRender(context::SceneContext& context) = 0;
};
} // namespace ssg