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
---@field Connect fun(self: Signal<T>, callback: fun(event: T)): Connection
