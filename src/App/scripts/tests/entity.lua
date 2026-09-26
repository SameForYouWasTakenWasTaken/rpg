local Test = require("scripts.tests.test")

-- NOTE: entity:add/has/remove/get dispatch through find_ops(name), which
-- resolves a component-type name (e.g. "Transform", "Sprite", ...) to its
-- registered ops table. Those component names aren't defined in the files
-- you shared, so exercising add/has/remove/get genuinely needs a real
-- component name from your project. Swap "YourComponentName" below for one
-- that's actually registered, or delete that block if you'd rather test it
-- separately.

print("======= ENTITY TEST =======")


Test.run("entity.create() returns a valid entity", function()
    local e = engine.entity.create()
    Test.assert_not_nil(e)
    Test.assert_true(e:valid())
    e:destroy()
end)

Test.run("Entity becomes invalid after destroy()", function()
    local e = engine.entity.create()
    Test.assert_true(e:valid())

    e:destroy()
    Test.assert_false(e:valid())
end)

Test.run("destroy() on an already-destroyed entity is a no-op", function()
    local e = engine.entity.create()
    e:destroy()
    Test.assert_false(e:valid())

    -- Entity::destroy() guards on is_valid(), so calling it again shouldn't error.
    e:destroy()
    Test.assert_false(e:valid())
end)

Test.run("Entity equality compares the same underlying entity", function()
    local a = engine.entity.create()
    local b = engine.entity.create()

    Test.assert_true(a == a)
    Test.assert_false(a == b)

    a:destroy()
    b:destroy()
end)

Test.run("entity.create() produces distinct entities", function()
    local a = engine.entity.create()
    local b = engine.entity.create()

    Test.assert_false(a == b)

    a:destroy()
    b:destroy()
end)

-- Uncomment and fill in a real component name to test add/has/remove/get:
--
-- Test.run("Entity:add/has/remove a component", function()
--     local e = entity.create()
--
--     Test.assert_false(e:has("YourComponentName"))
--
--     e:add("YourComponentName", {})
--     Test.assert_true(e:has("YourComponentName"))
--
--     local comp = e:get("YourComponentName")
--     Test.assert_not_nil(comp)
--
--     e:remove("YourComponentName")
--     Test.assert_false(e:has("YourComponentName"))
--
--     e:destroy()
-- end)
