#pragma once

#include "Rendering/Renderer.hpp"
#include "Rendering/Window.hpp"
#include <SFML/Graphics.hpp>

namespace ssg {

struct ApplicationContext
{
    ApplicationContext(const ApplicationContext&) = delete;
    ApplicationContext(ApplicationContext&&) = delete;
    ApplicationContext& operator=(const ApplicationContext&) = delete;
    ApplicationContext& operator=(ApplicationContext&&) = delete;
    
    ApplicationContext(
        Window& w
    ) : MainWindow(w) {}

    Window& MainWindow;
};

class Application {
public:
    Application();

    void Run();
    void Shutdown();
    void HandleEvents(Window& window);
private:
    Renderer m_renderer;
    Window m_window{WindowSettings{}};
};

} // namespace ssg
