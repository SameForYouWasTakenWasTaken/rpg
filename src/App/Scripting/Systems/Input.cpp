#include "Input.hpp"

#include <algorithm>
#include <iostream>
#include <utility>

#include "Engine.hpp"
#include "Systems/InputGate.hpp"
#include "Systems/InputSystem.hpp"

namespace ssg::lua::api::input
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
            std::cerr << "[lua] input callback error: " << error.what() << '\n';
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

InputAPI::InputAPI(EngineContext& context) : m_Context(context) {}

void InputAPI::Bind()
{
    auto& eventBus = m_Context.engine.GetEventBus();
    eventBus.Sink<KeyPressedEvent>().connect<&InputAPI::OnKeyPressed>(this);
    eventBus.Sink<KeyReleasedEvent>().connect<&InputAPI::OnKeyReleased>(this);
    eventBus.Sink<KeyRepeatedEvent>().connect<&InputAPI::OnKeyRepeated>(this);
    eventBus.Sink<MouseButtonPressedEvent>().connect<&InputAPI::OnMouseButtonPressed>(this);
    eventBus.Sink<MouseButtonReleasedEvent>().connect<&InputAPI::OnMouseButtonReleased>(this);
    eventBus.Sink<MouseMovedEvent>().connect<&InputAPI::OnMouseMoved>(this);
    eventBus.Sink<MouseWheelScrolledEvent>().connect<&InputAPI::OnMouseWheelScrolled>(this);
}

bool InputAPI::IsKeyDown(int key) const
{
    return !m_Context.engine.GetInputGate().WantsKeyboard() &&
           Input::IsKeyDown(static_cast<Input::Key>(key));
}

bool InputAPI::IsKeyUp(int key) const { return !IsKeyDown(key); }

bool InputAPI::IsMouseButtonDown(int button) const
{
    return !m_Context.engine.GetInputGate().WantsMouse() &&
           Input::IsMouseButtonDown(static_cast<Input::MouseButton>(button));
}

bool InputAPI::IsMouseButtonUp(int button) const { return !IsMouseButtonDown(button); }

Vec2 InputAPI::GetMousePosition() const { return Input::GetMousePosition(); }

void InputAPI::OnKeyPressed(const KeyPressedEvent& event)
{
    if (m_Context.engine.GetInputGate().WantsKeyboard())
        return;

    sol::state_view state{m_Context.engine.GetScriptEngine().getState().lua_state()};
    auto data =
        state.create_table_with("key", static_cast<int>(event.key), "alt", event.alt, "control",
                                event.control, "shift", event.shift, "system", event.system);
    m_KeyPressed.Fire(data);
}

void InputAPI::OnKeyReleased(const KeyReleasedEvent& event)
{
    if (m_Context.engine.GetInputGate().WantsKeyboard())
        return;

    sol::state_view state{m_Context.engine.GetScriptEngine().getState().lua_state()};
    auto data =
        state.create_table_with("key", static_cast<int>(event.key), "alt", event.alt, "control",
                                event.control, "shift", event.shift, "system", event.system);
    m_KeyReleased.Fire(data);
}

void InputAPI::OnKeyRepeated(const KeyRepeatedEvent& event)
{
    if (m_Context.engine.GetInputGate().WantsKeyboard())
        return;

    sol::state_view state{m_Context.engine.GetScriptEngine().getState().lua_state()};
    m_KeyRepeated.Fire(state.create_table_with("key", static_cast<int>(event.key)));
}

void InputAPI::OnMouseButtonPressed(const MouseButtonPressedEvent& event)
{
    if (m_Context.engine.GetInputGate().WantsMouse())
        return;

    sol::state_view state{m_Context.engine.GetScriptEngine().getState().lua_state()};
    m_MouseButtonPressed.Fire(state.create_table_with("button", static_cast<int>(event.button), "x",
                                                      event.x, "y", event.y, "position",
                                                      Vec2{event.x, event.y}));
}

void InputAPI::OnMouseButtonReleased(const MouseButtonReleasedEvent& event)
{
    if (m_Context.engine.GetInputGate().WantsMouse())
        return;

    sol::state_view state{m_Context.engine.GetScriptEngine().getState().lua_state()};
    m_MouseButtonReleased.Fire(state.create_table_with("button", static_cast<int>(event.button),
                                                       "x", event.x, "y", event.y, "position",
                                                       Vec2{event.x, event.y}));
}

void InputAPI::OnMouseMoved(const MouseMovedEvent& event)
{
    if (m_Context.engine.GetInputGate().WantsMouse())
        return;

    sol::state_view state{m_Context.engine.GetScriptEngine().getState().lua_state()};
    m_MouseMoved.Fire(
        state.create_table_with("x", event.x, "y", event.y, "position", Vec2{event.x, event.y}));
}

void InputAPI::OnMouseWheelScrolled(const MouseWheelScrolledEvent& event)
{
    if (m_Context.engine.GetInputGate().WantsMouse())
        return;

    sol::state_view state{m_Context.engine.GetScriptEngine().getState().lua_state()};
    m_MouseWheelScrolled.Fire(state.create_table_with("wheel", static_cast<int>(event.wheel),
                                                      "delta", event.delta, "x", event.x, "y",
                                                      event.y, "position", Vec2{event.x, event.y}));
}

namespace
{
void set_key_constants(sol::table& keys)
{
#define SSG_LUA_KEY(name) keys[#name] = static_cast<int>(Input::Key::name)
    SSG_LUA_KEY(Unknown);
    SSG_LUA_KEY(A);
    SSG_LUA_KEY(B);
    SSG_LUA_KEY(C);
    SSG_LUA_KEY(D);
    SSG_LUA_KEY(E);
    SSG_LUA_KEY(F);
    SSG_LUA_KEY(G);
    SSG_LUA_KEY(H);
    SSG_LUA_KEY(I);
    SSG_LUA_KEY(J);
    SSG_LUA_KEY(K);
    SSG_LUA_KEY(L);
    SSG_LUA_KEY(M);
    SSG_LUA_KEY(N);
    SSG_LUA_KEY(O);
    SSG_LUA_KEY(P);
    SSG_LUA_KEY(Q);
    SSG_LUA_KEY(R);
    SSG_LUA_KEY(S);
    SSG_LUA_KEY(T);
    SSG_LUA_KEY(U);
    SSG_LUA_KEY(V);
    SSG_LUA_KEY(W);
    SSG_LUA_KEY(X);
    SSG_LUA_KEY(Y);
    SSG_LUA_KEY(Z);
    SSG_LUA_KEY(Num0);
    SSG_LUA_KEY(Num1);
    SSG_LUA_KEY(Num2);
    SSG_LUA_KEY(Num3);
    SSG_LUA_KEY(Num4);
    SSG_LUA_KEY(Num5);
    SSG_LUA_KEY(Num6);
    SSG_LUA_KEY(Num7);
    SSG_LUA_KEY(Num8);
    SSG_LUA_KEY(Num9);
    SSG_LUA_KEY(Escape);
    SSG_LUA_KEY(LControl);
    SSG_LUA_KEY(LShift);
    SSG_LUA_KEY(LAlt);
    SSG_LUA_KEY(LSystem);
    SSG_LUA_KEY(RControl);
    SSG_LUA_KEY(RShift);
    SSG_LUA_KEY(RAlt);
    SSG_LUA_KEY(RSystem);
    SSG_LUA_KEY(Menu);
    SSG_LUA_KEY(LBracket);
    SSG_LUA_KEY(RBracket);
    SSG_LUA_KEY(Semicolon);
    SSG_LUA_KEY(Comma);
    SSG_LUA_KEY(Period);
    SSG_LUA_KEY(Apostrophe);
    SSG_LUA_KEY(Slash);
    SSG_LUA_KEY(Backslash);
    SSG_LUA_KEY(Grave);
    SSG_LUA_KEY(Equal);
    SSG_LUA_KEY(Hyphen);
    SSG_LUA_KEY(Space);
    SSG_LUA_KEY(Enter);
    SSG_LUA_KEY(Backspace);
    SSG_LUA_KEY(Tab);
    SSG_LUA_KEY(PageUp);
    SSG_LUA_KEY(PageDown);
    SSG_LUA_KEY(End);
    SSG_LUA_KEY(Home);
    SSG_LUA_KEY(Insert);
    SSG_LUA_KEY(Delete);
    SSG_LUA_KEY(Add);
    SSG_LUA_KEY(Subtract);
    SSG_LUA_KEY(Multiply);
    SSG_LUA_KEY(Divide);
    SSG_LUA_KEY(Left);
    SSG_LUA_KEY(Right);
    SSG_LUA_KEY(Up);
    SSG_LUA_KEY(Down);
    SSG_LUA_KEY(Numpad0);
    SSG_LUA_KEY(Numpad1);
    SSG_LUA_KEY(Numpad2);
    SSG_LUA_KEY(Numpad3);
    SSG_LUA_KEY(Numpad4);
    SSG_LUA_KEY(Numpad5);
    SSG_LUA_KEY(Numpad6);
    SSG_LUA_KEY(Numpad7);
    SSG_LUA_KEY(Numpad8);
    SSG_LUA_KEY(Numpad9);
    SSG_LUA_KEY(F1);
    SSG_LUA_KEY(F2);
    SSG_LUA_KEY(F3);
    SSG_LUA_KEY(F4);
    SSG_LUA_KEY(F5);
    SSG_LUA_KEY(F6);
    SSG_LUA_KEY(F7);
    SSG_LUA_KEY(F8);
    SSG_LUA_KEY(F9);
    SSG_LUA_KEY(F10);
    SSG_LUA_KEY(F11);
    SSG_LUA_KEY(F12);
    SSG_LUA_KEY(F13);
    SSG_LUA_KEY(F14);
    SSG_LUA_KEY(F15);
    SSG_LUA_KEY(Pause);
#undef SSG_LUA_KEY
}
} // namespace

void register_all(sol::table& engine, EngineContext& context)
{
    auto input = std::make_shared<InputAPI>(context);
    input->Bind();

    sol::state_view state{engine.lua_state()};

    // Signal + connection types
    state.new_usertype<Signal>("InputSignal", sol::no_constructor, "Connect", &Signal::Connect);
    state.new_usertype<Connection>("InputConnection", sol::no_constructor, "Disconnect",
                                   &Connection::Disconnect, "Connected", &Connection::Connected);

    // Input related stuff
    auto inputTable = engine.create_named("input");
    inputTable.set_function("is_key_down", [input](int key) { return input->IsKeyDown(key); });
    inputTable.set_function("is_key_up", [input](int key) { return input->IsKeyUp(key); });
    inputTable.set_function("is_mouse_button_down",
                            [input](int button) { return input->IsMouseButtonDown(button); });
    inputTable.set_function("is_mouse_button_up",
                            [input](int button) { return input->IsMouseButtonUp(button); });
    inputTable.set_function("mouse_position", [input] { return input->GetMousePosition(); });
    inputTable["key_pressed"] = input->KeyPressed();
    inputTable["key_released"] = input->KeyReleased();
    inputTable["key_repeated"] = input->KeyRepeated();
    inputTable["mouse_button_pressed"] = input->MouseButtonPressed();
    inputTable["mouse_button_released"] = input->MouseButtonReleased();
    inputTable["mouse_moved"] = input->MouseMoved();
    inputTable["mouse_wheel_scrolled"] = input->MouseWheelScrolled();

    // Create key enums
    auto keys = state.create_table();
    set_key_constants(keys);
    inputTable["Key"] = keys;

    auto buttons = state.create_table_with("Left", static_cast<int>(Input::MouseButton::Left),
                                           "Right", static_cast<int>(Input::MouseButton::Right),
                                           "Middle", static_cast<int>(Input::MouseButton::Middle),
                                           "Extra1", static_cast<int>(Input::MouseButton::Extra1),
                                           "Extra2", static_cast<int>(Input::MouseButton::Extra2));
    inputTable["MouseButton"] = buttons;

    inputTable["MouseWheel"] =
        state.create_table_with("Vertical", static_cast<int>(sf::Mouse::Wheel::Vertical),
                                "Horizontal", static_cast<int>(sf::Mouse::Wheel::Horizontal));
}

} // namespace ssg::lua::api::input
