#pragma once

#include <SFML/Graphics.hpp>

#include "EngineContext.hpp"
#include "Rendering/Renderer.hpp"
#include "Rendering/Window.hpp"

namespace ssg
{

struct ApplicationContext
{
    ApplicationContext(const ApplicationContext&) = delete;
    ApplicationContext(ApplicationContext&&) = delete;
    ApplicationContext& operator=(const ApplicationContext&) = delete;
    ApplicationContext& operator=(ApplicationContext&&) = delete;

    ApplicationContext(Window& w) : MainWindow(w) {}

    Window& MainWindow;
};

class Application
{
  public:
    Application(EngineContext& engineContext);

    Application(const Application& other) = delete;
    Application(Application&& other) noexcept = delete;
    Application& operator=(const Application& other) = delete;
    Application& operator=(Application&& other) noexcept = delete;

    void Run();
    void Shutdown();

    void HandleEvents();

  private:
    EngineContext& m_EngineContext;
    Window m_Window{m_EngineContext, WindowSettings{}};
};

} // namespace ssg
