---@meta

---@class Connection
local Connection = {}

---Stops this callback from receiving future events. Safe to call more than once.
function Connection:Disconnect()
end

---@return boolean
function Connection:Connected()
end

---@class Signal<T>
local Signal = {}

---@generic T
---@param callback fun(event: T)
---@return Connection
function Signal:Connect(callback)
end
