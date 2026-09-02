#pragma once

#include <atomic>

#include "Events/EventBus.hpp"
#include "Logger.hpp"
#include "Rendering/Renderer.hpp"
#include "Systems/AssetManager.hpp"
#include "Systems/InputSystem.hpp"

#define SSG_ENGINE_HPP_INCLUDED

namespace ssg
{

class Engine
{
  public:
    Engine() = default;

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    void initialize();
    void terminate();

    bool isRunning() const;

    [[nodiscard]] const EventBus& GetEventBus() const { return eventBus; }
    [[nodiscard]] EventBus& GetEventBus() { return eventBus; }

    [[nodiscard]] AssetManager& GetAssetManager() { return assetManager; }
    [[nodiscard]] const AssetManager& GetAssetManager() const { return assetManager; }

    [[nodiscard]] const log::Logger& GetLogger() const { return logger; }
    [[nodiscard]] log::Logger& GetLogger() { return logger; }

    [[nodiscard]] const Input& GetInputSystem() const { return inputSystem; }
    [[nodiscard]] Input& GetInputSystem() { return inputSystem; }

    [[nodiscard]] Renderer& GetRenderer() { return m_renderer; }
    [[nodiscard]] const Renderer& GetRenderer() const { return m_renderer; }

  private:
    EventBus eventBus;
    AssetManager assetManager;
    log::Logger logger;

    Input inputSystem{eventBus};

    Renderer m_renderer;

    log::ConsoleSink m_ConsoleSink;
    std::atomic<bool> m_running{false};
};

} // namespace ssg
