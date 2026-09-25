---@meta

---@class InputConnection
local InputConnection = {}

---Stops this callback from receiving future events. Safe to call more than once.
function InputConnection:Disconnect()
end

---@return boolean
function InputConnection:Connected()
end

---@class InputSignal<T>
local InputSignal = {}

---@generic T
---@param callback fun(event: T)
---@return InputConnection
function InputSignal:Connect(callback)
end

---@class InputKey
---@field Unknown integer
---@field A integer
---@field B integer
---@field C integer
---@field D integer
---@field E integer
---@field F integer
---@field G integer
---@field H integer
---@field I integer
---@field J integer
---@field K integer
---@field L integer
---@field M integer
---@field N integer
---@field O integer
---@field P integer
---@field Q integer
---@field R integer
---@field S integer
---@field T integer
---@field U integer
---@field V integer
---@field W integer
---@field X integer
---@field Y integer
---@field Z integer
---@field Num0 integer
---@field Num1 integer
---@field Num2 integer
---@field Num3 integer
---@field Num4 integer
---@field Num5 integer
---@field Num6 integer
---@field Num7 integer
---@field Num8 integer
---@field Num9 integer
---@field Escape integer
---@field LControl integer
---@field LShift integer
---@field LAlt integer
---@field LSystem integer
---@field RControl integer
---@field RShift integer
---@field RAlt integer
---@field RSystem integer
---@field Menu integer
---@field LBracket integer
---@field RBracket integer
---@field Semicolon integer
---@field Comma integer
---@field Period integer
---@field Apostrophe integer
---@field Slash integer
---@field Backslash integer
---@field Grave integer
---@field Equal integer
---@field Hyphen integer
---@field Space integer
---@field Enter integer
---@field Backspace integer
---@field Tab integer
---@field PageUp integer
---@field PageDown integer
---@field End integer
---@field Home integer
---@field Insert integer
---@field Delete integer
---@field Add integer
---@field Subtract integer
---@field Multiply integer
---@field Divide integer
---@field Left integer
---@field Right integer
---@field Up integer
---@field Down integer
---@field Numpad0 integer
---@field Numpad1 integer
---@field Numpad2 integer
---@field Numpad3 integer
---@field Numpad4 integer
---@field Numpad5 integer
---@field Numpad6 integer
---@field Numpad7 integer
---@field Numpad8 integer
---@field Numpad9 integer
---@field F1 integer
---@field F2 integer
---@field F3 integer
---@field F4 integer
---@field F5 integer
---@field F6 integer
---@field F7 integer
---@field F8 integer
---@field F9 integer
---@field F10 integer
---@field F11 integer
---@field F12 integer
---@field F13 integer
---@field F14 integer
---@field F15 integer
---@field Pause integer

---@class InputMouseButton
---@field Left integer
---@field Right integer
---@field Middle integer
---@field Extra1 integer
---@field Extra2 integer

---@class InputMouseWheel
---@field Vertical integer
---@field Horizontal integer

---@alias InputKeyCode integer
---@alias InputMouseButtonCode integer
---@alias InputMouseWheelCode integer

---@class KeyInputEvent
---@field key InputKeyCode
---@field alt boolean
---@field control boolean
---@field shift boolean
---@field system boolean

---@class KeyRepeatedInputEvent
---@field key InputKeyCode

---@class MouseButtonInputEvent
---@field button InputMouseButtonCode
---@field x number
---@field y number
---@field position Vec2

---@class MouseMovedInputEvent
---@field x number
---@field y number
---@field position Vec2

---@class MouseWheelInputEvent: MouseMovedInputEvent
---@field wheel InputMouseWheelCode
---@field delta number


engine = {}
---@class engine.input
engine.input = {}

---@type InputKey
engine.input.Key = {}
---@type InputMouseButton
engine.input.MouseButton = {}
---@type InputMouseWheel
engine.input.MouseWheel = {}

---@param key InputKeyCode
---@return boolean
function engine.input.is_key_down(key)
end

---@param key InputKeyCode
---@return boolean
function engine.input.is_key_up(key)
end

---@param button InputMouseButtonCode
---@return boolean
function engine.input.is_mouse_button_down(button)
end

---@param button InputMouseButtonCode
---@return boolean
function engine.input.is_mouse_button_up(button)
end

---@return Vec2
function engine.input.mouse_position()
end

---@type InputSignal<KeyInputEvent>
engine.input.key_pressed = nil
---@type InputSignal<KeyInputEvent>
engine.input.key_released = nil
---@type InputSignal<KeyRepeatedInputEvent>
engine.input.key_repeated = nil
---@type InputSignal<MouseButtonInputEvent>
engine.input.mouse_button_pressed = nil
---@type InputSignal<MouseButtonInputEvent>
engine.input.mouse_button_released = nil
---@type InputSignal<MouseMovedInputEvent>
engine.input.mouse_moved = nil
---@type InputSignal<MouseWheelInputEvent>
engine.input.mouse_wheel_scrolled = nil
