#include "Engine.hpp"

#include "Logger.hpp"
#include "Rendering/Window.hpp"

namespace ssg
{

Engine& Engine::instance()
{
    static Engine engine;
    return engine;
}

void Engine::initialize()
{
    m_running.store(true);
    logger.AddSink(std::make_unique<log::ConsoleSink>(m_ConsoleSink));
}

void Engine::terminate() { m_running.store(false); }

bool Engine::isRunning() const { return m_running.load(); }

} // namespace ssg
