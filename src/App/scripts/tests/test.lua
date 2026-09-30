---@meta

local Test = {}

local function fail(message)
    error(message, 2)
end

function Test.assert(condition, message)
    if not condition then
        fail(message or "Assertion failed")
    end
end

function Test.assert_equal(actual, expected, message)
    if actual ~= expected then
        fail(
            (message or "Values are not equal")
            .. "\n  expected: " .. tostring(expected)
            .. "\n  actual:   " .. tostring(actual)
        )
    end
end

function Test.assert_not_equal(actual, expected, message)
    if actual == expected then
        fail(
            (message or "Values are equal")
            .. "\n  value: " .. tostring(actual)
        )
    end
end

function Test.assert_true(value, message)
    Test.assert(value == true, message or "Expected true, got " .. tostring(value))
end

function Test.assert_false(value, message)
    Test.assert(value == false, message or "Expected false, got " .. tostring(value))
end

function Test.assert_nil(value, message)
    Test.assert(value == nil, message or "Expected nil, got " .. tostring(value))
end

function Test.assert_not_nil(value, message)
    Test.assert(value ~= nil, message or "Expected non-nil value")
end

function Test.assert_error(fn, message)
    local ok = pcall(fn)

    if ok then
        fail(message or "Expected function to throw an error")
    end
end

function Test.run(name, fn)
    print("[TEST] " .. name)

    local ok, err = pcall(fn)

    if not ok then
        error("[FAIL] " .. name .. "\n" .. tostring(err), 0)
    end

    print("[PASS] " .. name)
end

return Test
