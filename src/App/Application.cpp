#include "Application.hpp"
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
#include "Rendering/Window.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "Rendering/Atlas.hpp"
#include "SceneStack.hpp"
#include "Scenes/GameScene.hpp"

#include <optional>

namespace ssg {

Application::Application()
{
}

void Application::Run() 
{   
    Engine& engine = Engine::instance();
    auto& eventBus = engine.eventBus;
    m_window.SetSettings(
        {
            .Width = 600,
            .Height = 800,
            .Framerate = 200,
            .title = "Game!"
        }
    );

    ApplicationContext Context = (
        m_window
    );

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
        HandleEvents(m_window);

        m_window.Clear(sf::Color::Black);
        
        m_renderer.Begin();
        stack.Update(dt, Context);
        stack.Render(m_renderer, Context);
        m_renderer.End(m_window);

        m_window.Display();

    }

    Shutdown();
}

void Application::HandleEvents(Window& window)
{
    auto& eventBus = Engine::instance().eventBus;
    while (const std::optional event = window.PollSFMLEvents())
    {
        if (EventBus::IsSFMLEvent<sf::Event::Closed>(event))
            eventBus.Queue<WindowCloseEvent>();

        if (auto pEvent = EventBus::IsSFMLEvent<sf::Event::Resized>(event))
            eventBus.Queue<WindowResizeEvent>(pEvent->size.x, pEvent->size.y);

        if (auto pEvent = EventBus::IsSFMLEvent<sf::Event::KeyPressed>(event))
            eventBus.Queue<KeyPressedEvent>(pEvent->code, pEvent->alt, pEvent->control, pEvent->shift, pEvent->system);

        if (auto pEvent = EventBus::IsSFMLEvent<sf::Event::KeyReleased>(event))
            eventBus.Queue<KeyReleasedEvent>(pEvent->code, pEvent->alt, pEvent->control, pEvent->shift, pEvent->system);

        if (auto pEvent = EventBus::IsSFMLEvent<sf::Event::MouseButtonPressed>(event))
            eventBus.Queue<MouseButtonPressedEvent>(pEvent->button, static_cast<float>(pEvent->position.x), static_cast<float>(pEvent->position.y));

        if (auto pEvent = EventBus::IsSFMLEvent<sf::Event::MouseButtonReleased>(event))
            eventBus.Queue<MouseButtonReleasedEvent>(pEvent->button, static_cast<float>(pEvent->position.x), static_cast<float>(pEvent->position.y));

        if (auto pEvent = EventBus::IsSFMLEvent<sf::Event::MouseMoved>(event))
            eventBus.Queue<MouseMovedEvent>(static_cast<float>(pEvent->position.x), static_cast<float>(pEvent->position.y));

        if (auto pEvent = EventBus::IsSFMLEvent<sf::Event::MouseWheelScrolled>(event))
            eventBus.Queue<MouseWheelScrolledEvent>(pEvent->wheel, pEvent->delta, static_cast<float>(pEvent->position.x), static_cast<float>(pEvent->position.y));

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

} // namespace ssg)
