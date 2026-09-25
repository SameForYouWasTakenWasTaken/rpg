#pragma once

#include <cstdint>
#include <memory>
#include <sol/sol.hpp>
#include <vector>

#include "EngineContext.hpp"
#include "Events/KeyPressedEvent.hpp"
#include "Events/KeyReleasedEvent.hpp" sss
#include "Events/KeyRepeatedEvent.hpp"
#include "Events/MouseButtonPressedEvent.hpp"
#include "Events/MouseButtonReleasedEvent.hpp"
#include "Events/MouseMovedEvent.hpp"
#include "Events/MouseWheelScrolledEvent.hpp"
#include "Types.hpp"

namespace ssg::lua::api::input
{

class Connection;

class Signal
{
  public:
    Signal();

    [[nodiscard]] Connection Connect(sol::protected_function callback) const;

  private:
    struct State;
    std::shared_ptr<State> m_State;

    void Fire(const sol::table& event) const;

    friend class Connection;
    friend class InputAPI;
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

class InputAPI
{
  public:
    explicit InputAPI(EngineContext& context);

    void Bind();

    [[nodiscard]] bool IsKeyDown(int key) const;
    [[nodiscard]] bool IsKeyUp(int key) const;
    [[nodiscard]] bool IsMouseButtonDown(int button) const;
    [[nodiscard]] bool IsMouseButtonUp(int button) const;
    [[nodiscard]] Vec2 GetMousePosition() const;

    [[nodiscard]] Signal KeyPressed() const { return m_KeyPressed; }
    [[nodiscard]] Signal KeyReleased() const { return m_KeyReleased; }
    [[nodiscard]] Signal KeyRepeated() const { return m_KeyRepeated; }
    [[nodiscard]] Signal MouseButtonPressed() const { return m_MouseButtonPressed; }
    [[nodiscard]] Signal MouseButtonReleased() const { return m_MouseButtonReleased; }
    [[nodiscard]] Signal MouseMoved() const { return m_MouseMoved; }
    [[nodiscard]] Signal MouseWheelScrolled() const { return m_MouseWheelScrolled; }

  private:
    EngineContext& m_Context;
    Signal m_KeyPressed;
    Signal m_KeyReleased;
    Signal m_KeyRepeated;
    Signal m_MouseButtonPressed;
    Signal m_MouseButtonReleased;
    Signal m_MouseMoved;
    Signal m_MouseWheelScrolled;

    void OnKeyPressed(const KeyPressedEvent& event);
    void OnKeyReleased(const KeyReleasedEvent& event);
    void OnKeyRepeated(const KeyRepeatedEvent& event);
    void OnMouseButtonPressed(const MouseButtonPressedEvent& event);
    void OnMouseButtonReleased(const MouseButtonReleasedEvent& event);
    void OnMouseMoved(const MouseMovedEvent& event);
    void OnMouseWheelScrolled(const MouseWheelScrolledEvent& event);
};

void register_all(sol::table& engine, EngineContext& context);

} // namespace ssg::lua::api::input
