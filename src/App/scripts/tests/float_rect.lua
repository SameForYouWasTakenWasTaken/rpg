local Test = require("scripts.tests.test")

print("======= FLOATRECT TEST =======")

Test.run("FloatRect() default constructor gives a zeroed rect", function()
    local r = FloatRect()
    Test.assert_equal(r.x, 0)
    Test.assert_equal(r.y, 0)
    Test.assert_equal(r.width, 0)
    Test.assert_equal(r.height, 0)
end)

Test.run("FloatRect(x, y, w, h) sets all fields", function()
    local r = FloatRect(1, 2, 3, 4)
    Test.assert_equal(r.x, 1)
    Test.assert_equal(r.y, 2)
    Test.assert_equal(r.width, 3)
    Test.assert_equal(r.height, 4)
end)

Test.run("FloatRect fields are independently settable", function()
    local r = FloatRect(0, 0, 0, 0)
    r.x = 5
    r.y = 6
    r.width = 7
    r.height = 8
    Test.assert_equal(r.x, 5)
    Test.assert_equal(r.y, 6)
    Test.assert_equal(r.width, 7)
    Test.assert_equal(r.height, 8)
end)

Test.run("FloatRect:contains detects points inside and outside", function()
    local r = FloatRect(0, 0, 10, 10)
    Test.assert_true(r:contains(5, 5))
    Test.assert_true(r:contains(0, 0))
    Test.assert_false(r:contains(15, 5))
    Test.assert_false(r:contains(-1, 5))
end)

Test.run("FloatRect:intersects detects overlap", function()
    local a = FloatRect(0, 0, 10, 10)
    local b = FloatRect(5, 5, 10, 10)
    local c = FloatRect(100, 100, 5, 5)

    Test.assert_true(a:intersects(b))
    Test.assert_false(a:intersects(c))
end)

Test.run("FloatRect:intersection returns the overlapping rect", function()
    local a = FloatRect(0, 0, 10, 10)
    local b = FloatRect(5, 5, 10, 10)

    local i = a:intersection(b)
    Test.assert_not_nil(i)
    Test.assert_equal(i.x, 5)
    Test.assert_equal(i.y, 5)
    Test.assert_equal(i.width, 5)
    Test.assert_equal(i.height, 5)
end)

Test.run("FloatRect:intersection returns nil when there is no overlap", function()
    local a = FloatRect(0, 0, 10, 10)
    local c = FloatRect(100, 100, 5, 5)

    local i = a:intersection(c)
    Test.assert_nil(i)
end)

Test.run("FloatRect:center returns two values (cx, cy)", function()
    local r = FloatRect(0, 0, 10, 20)
    local cx, cy = r:center()
    Test.assert_equal(cx, 5)
    Test.assert_equal(cy, 10)
end)

Test.run("FloatRect equality compares by value", function()
    local a = FloatRect(1, 2, 3, 4)
    local b = FloatRect(1, 2, 3, 4)
    local c = FloatRect(1, 2, 3, 5)

    Test.assert_true(a == b)
    Test.assert_false(a == c)
end)

Test.run("FloatRect tostring formats as FloatRect(x, y, w, h)", function()
    local r = FloatRect(1, 2, 3, 4)
    local s = tostring(r)
    Test.assert(s:match("^FloatRect%("), "expected string to start with 'FloatRect(', got: " .. s)
end)
