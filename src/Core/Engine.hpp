#pragma once

#include <atomic>

#include "Events/EventBus.hpp"
#include "Logger.hpp"
#include "Rendering/Window.hpp"
#include "Systems/AssetManager.hpp"
#include "Systems/InputSystem.hpp"

#define SSG_ENGINE_HPP_INCLUDED

namespace ssg
{

class Engine
{
  public:
    static Engine& instance();

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    void initialize();
    void terminate();

    bool isRunning() const;

    EventBus eventBus;
    AssetManager assetManager;
    log::Logger logger;
    Input inputSystem{eventBus};

  private:
    Engine() = default;

    std::atomic<bool> m_running{false};
    log::ConsoleSink m_ConsoleSink;
};

} // namespace ssg
