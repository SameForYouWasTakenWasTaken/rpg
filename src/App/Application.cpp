#include "Application.hpp"

#include <optional>

#include "Engine.hpp"
#include "Events/EventBus.hpp"
#include "Events/KeyPressedEvent.hpp"
#include "Events/KeyReleasedEvent.hpp"
#include "Events/MouseButtonPressedEvent.hpp"
#include "Events/MouseButtonReleasedEvent.hpp"
#include "Events/MouseMovedEvent.hpp"
#include "Events/MouseWheelScrolledEvent.hpp"
#include "Events/TextEnteredEvent.hpp"
#include "Events/WindowCloseEvent.hpp"
#include "Layers/GameLayer.hpp"
#include "Rendering/Atlas.hpp"
#include "Rendering/Window.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SceneStack.hpp"
#include "Scenes/GameScene.hpp"

namespace ssg
{

Application::Application() {}

void Application::Run()
{
    Engine& engine = Engine::instance();
    auto& eventBus = engine.eventBus;
    m_window.SetSettings({.Width = 600, .Height = 800, .Framerate = 200, .title = "Game!"});

    ApplicationContext Context = (m_window);

    SceneStack stack;

    auto scene = std::make_unique<GameScene>();
    scene->PushLayer(std::make_unique<GameLayer>());

    stack.Push(std::move(scene));

    sf::Clock clock;
    clock.start();
    while (m_window.IsOpen() && engine.isRunning())
    {
        float dt = clock.restart().asSeconds();
        eventBus.Update(); // Update events at the start of the frame

        HandleEvents();

        m_window.Clear(sf::Color::Black);

        m_renderer.Begin();

        stack.Update(dt, Context);
        stack.Render(m_renderer, Context);

        m_renderer.End(m_window);

        m_window.Display();
    }

    Shutdown();
}

void Application::HandleEvents()
{
    auto& engine = Engine::instance();
    auto& eventBus = engine.eventBus;
    auto& input = engine.inputSystem;

    while (const std::optional event = m_window.PollSFMLEvents())
    {
        // process input, such as mouse and key presses, releases, movement, etc
        input.ProcessEvents(event);
        if (EventBus::IsSFMLEvent<sf::Event::Closed>(event))
            eventBus.Queue<WindowCloseEvent>();

        if (auto pEvent = EventBus::IsSFMLEvent<sf::Event::Resized>(event))
            eventBus.Queue<WindowResizeEvent>(pEvent->size.x, pEvent->size.y);

        if (auto pEvent = EventBus::IsSFMLEvent<sf::Event::TextEntered>(event))
            eventBus.Queue<TextEnteredEvent>(static_cast<uint32_t>(pEvent->unicode));
    }
}

void Application::Shutdown()
{
    auto& engine = Engine::instance();
    engine.terminate();

    if (m_window.IsOpen())
    {
        m_window.Close();
    }
}

} // namespace ssg
