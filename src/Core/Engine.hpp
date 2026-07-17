#pragma once

#include "Events/EventBus.hpp"
#include "Rendering/Window.hpp"
#include "Systems/AssetManager.hpp"
#include <atomic>
#include <iterator>

namespace ssg {

class Engine {
public:
    static Engine& instance();

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    void initialize();
    void terminate();

    bool isRunning() const;

    EventBus eventBus;
    AssetManager assetManager;
private:
    Engine() = default;
    
    std::atomic<bool> m_running{false};
};

} // namespace ssg
