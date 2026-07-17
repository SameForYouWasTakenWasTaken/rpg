#pragma once

#include "Application.hpp"
#include "Rendering/Renderer.hpp"

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

    virtual void OnAttach() = 0;
    virtual void OnDetach() = 0;

    virtual void OnUpdate(float, ApplicationContext& context) = 0;
    virtual void OnRender(Renderer& renderer, ApplicationContext& context) = 0;
};
} // namespace ssg