#include "Engine.hpp"
#include "Rendering/Window.hpp"

namespace ssg {

Engine& Engine::instance() 
{
    static Engine engine;
    return engine;
}

void Engine::initialize() {
    m_running.store(true);
}

void Engine::terminate() {
    m_running.store(false);
}

bool Engine::isRunning() const {
    return m_running.load();
}

} // namespace ssg
