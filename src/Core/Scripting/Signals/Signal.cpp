#include "Signal.hpp"

#include <algorithm>
#include <iostream>
#include <utility>

namespace ssg::lua::api::signals
{

struct Signal::State
{
    struct Listener
    {
        std::uint64_t id;
        sol::protected_function callback;
    };

    std::vector<Listener> listeners;
    std::uint64_t nextId = 1;
};

Signal::Signal() : m_State(std::make_shared<State>()) {}

Connection Signal::Connect(sol::protected_function callback) const
{
    const std::uint64_t id = m_State->nextId++;
    m_State->listeners.push_back({id, std::move(callback)});
    return {m_State, id};
}

void Signal::Fire(const sol::table& event) const
{
    // Copying lets a callback disconnect itself (or another callback) safely.
    const auto listeners = m_State->listeners;
    for (const auto& listener : listeners)
    {
        const auto stillConnected =
            std::ranges::any_of(m_State->listeners, [&listener](const State::Listener& current)
                                { return current.id == listener.id; });
        if (!stillConnected)
            continue;

        sol::protected_function_result result = listener.callback(event);
        if (!result.valid())
        {
            sol::error error = result;
            std::cerr << "[lua] signal callback error: " << error.what() << '\n';
        }
    }
}

Connection::Connection(std::weak_ptr<Signal::State> state, std::uint64_t id)
    : m_State(std::move(state)), m_Id(id)
{
}

void Connection::Disconnect()
{
    const auto state = m_State.lock();
    if (!state)
        return;

    std::erase_if(state->listeners,
                  [this](const Signal::State::Listener& listener) { return listener.id == m_Id; });
    m_State.reset();
}

bool Connection::Connected() const
{
    const auto state = m_State.lock();
    return state &&
           std::ranges::any_of(state->listeners, [this](const Signal::State::Listener& listener)
                               { return listener.id == m_Id; });
}

void register_all(sol::state_view state)
{
    state.new_usertype<Signal>("Signal", sol::no_constructor, "Connect", &Signal::Connect);
    state.new_usertype<Connection>("Connection", sol::no_constructor, "Disconnect",
                                   &Connection::Disconnect, "Connected", &Connection::Connected);
}

} // namespace ssg::lua::api::signals
