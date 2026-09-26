local Test = require("tests.test")

Test.run("Camera default constructor", function()
    local camera = Camera.new()

    Test.assert_not_nil(camera)
    Test.assert_not_nil(camera.rotation)
    Test.assert_not_nil(camera.size)
    Test.assert_not_nil(camera.center)
    Test.assert_not_nil(camera.zoom)
end)

Test.run("Camera parameterized constructor", function()
    local size = Vec2.new(1280, 720)
    local center = Vec2.new(640, 360)

    local camera = Camera.new(center, size)

    Test.assert_equal(camera.size, size, "Size")
    Test.assert_equal(camera.center, center, "Center")
end)

Test.run("Camera zoom property", function()
    local camera = Camera.new()

    Test.assert_equal(camera.zoom, 1)

    camera.zoom = 2

    Test.assert_equal(camera.zoom, 2)
end)

Test.run("Camera size property", function()
    local camera = Camera.new()

    local size = Vec2.new(1920, 1080)

    camera.size = size

    Test.assert_equal(camera.size, size)
end)

Test.run("Camera center property", function()
    local camera = Camera.new()

    local center = Vec2.new(960, 540)

    camera.center = center

    Test.assert_equal(camera.center, center)
end)

Test.run("Camera movement", function()
    local camera = Camera.new()
    camera.center = Vec2.new(0, 0);
    camera:move(Vec2.new(100, 50))


    Test.assert_equal(camera.center, Vec2.new(100, 50))
end)

Test.run("Camera rotation", function()
    local camera = Camera.new()

    camera:rotate(10)

    Test.assert_not_nil(camera.rotation)
end)

Test.run("Camera zoom_by", function()
    local camera = Camera.new()

    local old_zoom = camera.zoom

    camera:zoom_by(5)

    Test.assert_not_equal(camera.zoom, old_zoom)
end)

print("[PASS] All Camera tests")
