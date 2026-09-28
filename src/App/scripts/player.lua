local input = engine.input
local assets = engine.assets

-- atlas creation
local atlas = engine.assets.load_texture_packer_atlas(
    "assets/Textures/Atlas/random.png",
    "assets/Textures/Atlas/random.json"
)

local region = atlas:get_region("dogbite")
local otherRegion = atlas:get_region("shocked")

local function createSpawnable(asset_handle, region)
    local e = engine.entity.create()
    e:add("transform", { world_position = Vec2.new(100, 100) })
    e:add("sprite", { size = Vec2.new(128, 128), z_index = 10, origin = Vec2.new(0.5, 0.5) })
    e:add("texture", { asset_handle = asset_handle, float_rect = region:get_dimensions() })

    return e
end

local someWeapon = createSpawnable(atlas.asset_handle, otherRegion)
local weaponSprite = someWeapon:get("sprite")
local weaponTransform = someWeapon:get("transform")
someWeapon:add("relationship")
weaponSprite.size = Vec2.new(32, 32)


-- create player
local player = createSpawnable(atlas.asset_handle, region)
player:add("inventory", { capacity = 20 })
player:add("relationship")

-- create map
local someMap = assets.map.load_tiled("data/maps/other_random_map/mapWithTree.tmj")
someMap:create() -- create the entities

local transform = player:get("transform")
local sprite = player:get("sprite")
local inventory = player:get("inventory")
if not transform then return end
if not sprite then return end
if not inventory then return end

print("inventory capacity:", inventory.capacity)
weaponTransform.position = transform.position + Vec2.new(20, -20)

-- set camera
local window = engine.Window
local cam = Camera.new(transform.world_position, window.size)
cam:zoom_by(0.5)
window:set_camera(cam)

-- set hierarchies

local hierarchy = engine.hierarchy
hierarchy.attach_child(player, someWeapon)
print("Is player root:", hierarchy.is_root(player))
print("Root of weapon is: ", hierarchy.get_root(someWeapon))

function update(dt)
    local speed = 100;
    if (input.is_key_down(input.Key.W)) then
        transform.position.y = transform.position.y - speed * dt
    end

    if (input.is_key_down(input.Key.S)) then
        transform.position.y = transform.position.y + speed * dt
    end

    if (input.is_key_down(input.Key.A)) then
        transform.position.x = transform.position.x - speed * dt
    end

    if (input.is_key_down(input.Key.D)) then
        transform.position.x = transform.position.x + speed * dt
    end

    cam.center = transform.world_position
    engine.Window:set_camera(cam);
end

-- events
window.closed:Connect(function(event)
    print("window closed")
end)

window.resized:Connect(function(event)
    print("window resized")
    print(event.width, event.height)
    cam.size = Vec2.new(event.width, event.height)
end)
