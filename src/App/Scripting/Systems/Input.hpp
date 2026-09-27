#pragma once

#include <sol/sol.hpp>

#include "../LuaContext.hpp"
#include "EngineContext.hpp"
#include "Events/KeyPressedEvent.hpp"
#include "Events/KeyReleasedEvent.hpp"
#include "Events/KeyRepeatedEvent.hpp"
#include "Events/MouseButtonPressedEvent.hpp"
#include "Events/MouseButtonReleasedEvent.hpp"
#include "Events/MouseMovedEvent.hpp"
#include "Events/MouseWheelScrolledEvent.hpp"
#include "Scripting/Signals/Signal.hpp"
#include "Types.hpp"

namespace ssg::lua::api::input
{

class InputAPI
{
  public:
    explicit InputAPI(LuaContext& lua_context);

    void Bind();

    [[nodiscard]] bool IsKeyDown(int key) const;
    [[nodiscard]] bool IsKeyUp(int key) const;
    [[nodiscard]] bool IsMouseButtonDown(int button) const;
    [[nodiscard]] bool IsMouseButtonUp(int button) const;
    [[nodiscard]] Vec2 GetMousePosition() const;

    [[nodiscard]] signals::Signal KeyPressed() const { return m_KeyPressed; }
    [[nodiscard]] signals::Signal KeyReleased() const { return m_KeyReleased; }
    [[nodiscard]] signals::Signal KeyRepeated() const { return m_KeyRepeated; }
    [[nodiscard]] signals::Signal MouseButtonPressed() const { return m_MouseButtonPressed; }
    [[nodiscard]] signals::Signal MouseButtonReleased() const { return m_MouseButtonReleased; }
    [[nodiscard]] signals::Signal MouseMoved() const { return m_MouseMoved; }
    [[nodiscard]] signals::Signal MouseWheelScrolled() const { return m_MouseWheelScrolled; }

  private:
    LuaContext& m_LuaContext;
    signals::Signal m_KeyPressed;
    signals::Signal m_KeyReleased;
    signals::Signal m_KeyRepeated;
    signals::Signal m_MouseButtonPressed;
    signals::Signal m_MouseButtonReleased;
    signals::Signal m_MouseMoved;
    signals::Signal m_MouseWheelScrolled;

    void OnKeyPressed(const KeyPressedEvent& event);
    void OnKeyReleased(const KeyReleasedEvent& event);
    void OnKeyRepeated(const KeyRepeatedEvent& event);
    void OnMouseButtonPressed(const MouseButtonPressedEvent& event);
    void OnMouseButtonReleased(const MouseButtonReleasedEvent& event);
    void OnMouseMoved(const MouseMovedEvent& event);
    void OnMouseWheelScrolled(const MouseWheelScrolledEvent& event);
};

void register_all(LuaContext& lua_context);

} // namespace ssg::lua::api::input
