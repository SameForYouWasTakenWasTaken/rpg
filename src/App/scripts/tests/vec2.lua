local Test = require("scripts.tests.test")

print("======= VEC2 TEST =======")

Test.run("Vec2.new() defaults to zero vector", function()
    local v = Vec2.new()
    Test.assert_equal(v.x, 0)
    Test.assert_equal(v.y, 0)
end)

Test.run("Vec2.new(s) fills both components with s", function()
    local v = Vec2.new(3)
    Test.assert_equal(v.x, 3)
    Test.assert_equal(v.y, 3)
end)

Test.run("Vec2.new(x, y) sets components independently", function()
    local v = Vec2.new(2, 5)
    Test.assert_equal(v.x, 2)
    Test.assert_equal(v.y, 5)
end)

Test.run("Vec2 x/y properties are settable", function()
    local v = Vec2.new(0, 0)
    v.x = 10
    v.y = -4
    Test.assert_equal(v.x, 10)
    Test.assert_equal(v.y, -4)
end)

Test.run("Vec2.length computes euclidean length", function()
    local v = Vec2.new(3, 4)
    Test.assert_equal(v:length(), 5)
end)

Test.run("Vec2.length_squared avoids the sqrt", function()
    local v = Vec2.new(3, 4)
    Test.assert_equal(v:length_squared(), 25)
end)

Test.run("Vec2.dot computes the dot product", function()
    local a = Vec2.new(1, 2)
    local b = Vec2.new(3, 4)
    Test.assert_equal(a:dot(b), 11)
end)

Test.run("Vec2.distance computes distance between two points", function()
    local a = Vec2.new(0, 0)
    local b = Vec2.new(3, 4)
    Test.assert_equal(a:distance(b), 5)
end)

Test.run("Vec2.lerp interpolates linearly", function()
    local a = Vec2.new(0, 0)
    local b = Vec2.new(10, 20)

    local mid = a:lerp(b, 0.5)
    Test.assert_equal(mid.x, 5)
    Test.assert_equal(mid.y, 10)

    local start = a:lerp(b, 0)
    Test.assert_equal(start.x, a.x)
    Test.assert_equal(start.y, a.y)

    local finish = a:lerp(b, 1)
    Test.assert_equal(finish.x, b.x)
    Test.assert_equal(finish.y, b.y)
end)

Test.run("Vec2.normalized returns a unit vector", function()
    local v = Vec2.new(3, 4)
    local n = v:normalized()
    Test.assert(math.abs(n:length() - 1) < 1e-5, "normalized vector should have length 1")
    Test.assert(math.abs(n.x - 0.6) < 1e-5)
    Test.assert(math.abs(n.y - 0.8) < 1e-5)
end)

Test.run("Vec2.normalized on the zero vector returns zero, not NaN", function()
    local v = Vec2.new(0, 0)
    local n = v:normalized()
    Test.assert_equal(n.x, 0)
    Test.assert_equal(n.y, 0)
end)

Test.run("Vec2 addition", function()
    local a = Vec2.new(1, 2)
    local b = Vec2.new(3, 4)
    local c = a + b
    Test.assert_equal(c.x, 4)
    Test.assert_equal(c.y, 6)
end)

Test.run("Vec2 subtraction", function()
    local a = Vec2.new(5, 7)
    local b = Vec2.new(2, 3)
    local c = a - b
    Test.assert_equal(c.x, 3)
    Test.assert_equal(c.y, 4)
end)

Test.run("Vec2 multiplication by scalar (vec * s and s * vec)", function()
    local v = Vec2.new(2, 3)

    local a = v * 2
    Test.assert_equal(a.x, 4)
    Test.assert_equal(a.y, 6)

    local b = 2 * v
    Test.assert_equal(b.x, 4)
    Test.assert_equal(b.y, 6)
end)

Test.run("Vec2 component-wise multiplication", function()
    local a = Vec2.new(2, 3)
    local b = Vec2.new(4, 5)
    local c = a * b
    Test.assert_equal(c.x, 8)
    Test.assert_equal(c.y, 15)
end)

Test.run("Vec2 division by scalar", function()
    local v = Vec2.new(10, 20)
    local c = v / 2
    Test.assert_equal(c.x, 5)
    Test.assert_equal(c.y, 10)
end)

Test.run("Vec2 component-wise division", function()
    local a = Vec2.new(10, 20)
    local b = Vec2.new(2, 5)
    local c = a / b
    Test.assert_equal(c.x, 5)
    Test.assert_equal(c.y, 4)
end)

Test.run("Vec2 unary minus negates both components", function()
    local v = Vec2.new(3, -4)
    local n = -v
    Test.assert_equal(n.x, -3)
    Test.assert_equal(n.y, 4)
end)

Test.run("Vec2 equality compares by value", function()
    local a = Vec2.new(1, 2)
    local b = Vec2.new(1, 2)
    local c = Vec2.new(1, 3)

    Test.assert_true(a == b)
    Test.assert_false(a == c)
end)

Test.run("Vec2 tostring formats as Vec2(x, y)", function()
    local v = Vec2.new(1, 2)
    local s = tostring(v)
    Test.assert(s:match("^Vec2%("), "expected string to start with 'Vec2(', got: " .. s)
end)
