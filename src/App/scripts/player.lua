local input = engine.input
local assets = engine.assets

-- atlas creation
local atlas = engine.assets.load_texture_packer_atlas(
    "assets/Textures/Atlas/random.png",
    "assets/Textures/Atlas/random.json"
)

local region = atlas:get_region("dogbite")
-- create player
local player = engine.entity.create()
player:add("transform", { position = Vec2.new(100, 100) })
player:add("sprite", { size = Vec2.new(128, 128), z_index = 10, origin = Vec2.new(0.5, 0.5) })
player:add("texture", { asset_handle = atlas.asset_handle, float_rect = region:get_dimensions() })

-- create map
local someMap = assets.map.load_tiled("data/maps/other_random_map/mapWithTree.tmj")
someMap:create() -- create the entities

local transform = player:get("transform")
local sprite = player:get("sprite")
if not transform then return end
if not sprite then return end

-- set camera
local window = engine.Window
local cam = Camera.new(transform.position, window.size)
cam:zoom_by(0.5)
window:set_camera(cam)

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

    cam.center = transform.position
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
