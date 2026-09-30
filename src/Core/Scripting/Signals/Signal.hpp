#pragma once

#include <cstdint>
#include <memory>
#include <sol/sol.hpp>
#include <vector>

namespace ssg::lua::api::input
{
class InputAPI;
}

namespace ssg::lua::api::signals
{

class Connection;

class Signal
{
  public:
    Signal();

    [[nodiscard]] Connection Connect(sol::protected_function callback) const;

    void Fire(const sol::table& event) const;

  private:
    struct State;
    std::shared_ptr<State> m_State;

    friend class Connection;
    friend class input::InputAPI;
};

class Connection
{
  public:
    Connection() = default;

    void Disconnect();
    [[nodiscard]] bool Connected() const;

  private:
    Connection(std::weak_ptr<Signal::State> state, std::uint64_t id);

    std::weak_ptr<Signal::State> m_State;
    std::uint64_t m_Id = 0;

    friend class Signal;
};

void register_all(sol::state_view state);

} // namespace ssg::lua::api::signals
