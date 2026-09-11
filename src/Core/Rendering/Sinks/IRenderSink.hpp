#pragma once
#include "Rendering/Window.hpp"

namespace ssg::rendering
{
class IRenderSink
{
  public:
    IRenderSink() = default;
    virtual ~IRenderSink() = default;

    virtual void Begin() = 0;
    virtual void End(Window& window) = 0;
};
} // namespace ssg::rendering