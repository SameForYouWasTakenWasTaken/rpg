#include "Application.hpp"

#include <optional>

#include "Engine.hpp"
#include "EngineContext.hpp"
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

Application::Application(EngineContext& context) : m_EngineContext(context) {}

void Application::Run()
{

    auto& engine = m_EngineContext.engine;
    auto& eventBus = engine.GetEventBus();
    auto& renderer = engine.GetRenderer();
    m_Window.SetSettings({.Width = 600, .Height = 800, .Framerate = 200, .title = "Game!"});

    ApplicationContext Context{m_Window};

    SceneStack stack;

    auto scene = std::make_unique<GameScene>();
    scene->PushLayer(std::make_unique<GameLayer>(m_EngineContext));

    stack.Push(std::move(scene));

    sf::Clock clock;
    clock.start();
    while (m_Window.IsOpen() && engine.isRunning())
    {
        float dt = clock.restart().asSeconds();
        eventBus.Update(); // Update events at the start of the frame

        HandleEvents();

        m_Window.Clear(sf::Color::Black);

        renderer.Begin();

        stack.Update(dt, Context);
        stack.Render(renderer, Context);

        renderer.End(m_Window);

        m_Window.Display();
    }

    Shutdown();
}

void Application::HandleEvents()
{
    auto& engine = m_EngineContext.engine;
    auto& eventBus = engine.GetEventBus();
    auto& input = engine.GetInputSystem();

    while (const std::optional event = m_Window.PollSFMLEvents())
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
    m_EngineContext.engine.terminate();

    if (m_Window.IsOpen())
    {
        m_Window.Close();
    }
}

} // namespace ssg
