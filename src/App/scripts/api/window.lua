---@meta

---@class Window
---@field size Vec2
local Window = {}

---@param framerate number
function Window:set_framerate(framerate) end

---@param enabled boolean
function Window:set_vsync(enabled) end

---@param path string
function Window:set_icon(path) end

---@param title string
function Window:set_title(title) end

---@return boolean
function Window:is_open() end

---@return boolean
function Window:is_focused() end

---@param camera Camera
function Window:set_camera(camera) end

---@type Window
engine.Window = Window

-- ======================== SIGNALS ========================
---@class ResizeEvent
---@field width number
---@field height number

---@class CloseEvent

---@class Window
---@field resized Signal<ResizeEvent>
---@field closed Signal<CloseEvent>
