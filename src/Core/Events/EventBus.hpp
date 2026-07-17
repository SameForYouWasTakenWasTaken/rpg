#pragma once

#include "Events/IEvent.hpp"
#include <concepts>
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

template <typename T>
concept TEvent = std::derived_from<T, ssg::IEvent>;

namespace ssg
{
class EventBus
{
public:
    EventBus() = default;
    ~EventBus(){m_Dispatcher.clear();}

    EventBus(const EventBus&) = delete;
    EventBus(EventBus&&) = delete;
    EventBus& operator=(const EventBus&) = delete;
    EventBus& operator=(EventBus&&) = delete;

    template <TEvent TEventFire, typename... Args>
    void Queue(Args&&... args);

    template <TEvent TEventFire, typename... Args>
    void Emit(Args&... args);

    template<TEvent TEventU>
    void Update() {m_Dispatcher.update<TEventU>();} // Update single event
    void Update() {m_Dispatcher.update();} // Update all events

    template <TEvent TEvent>
    auto Sink() {return m_Dispatcher.sink<TEvent>();}
    
    template <typename TEventReceive> // Don't use TEvent in place of typename here, since SFML events do not derive the IEvent interface
    static TEventReceive* IsSFMLEvent(std::optional<sf::Event> event)
    {
        if (auto* e = event->getIf<TEventReceive>())
            return e;
        return nullptr;
    }
private:
    entt::dispatcher m_Dispatcher;
};
template <TEvent TEventFire, typename... Args>
void EventBus::Queue(Args&&... args)
{
    m_Dispatcher.enqueue(TEventFire{std::forward<Args>(args)...});
}

template <TEvent TEventFire, typename... Args>
void EventBus::Emit(Args&... args)
{
    m_Dispatcher.trigger(TEventFire{std::forward<Args>(args)...});
}
} // ssg