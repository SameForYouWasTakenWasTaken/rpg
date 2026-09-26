local input = engine.input

local atlas = engine.assets.load_texture_packer_atlas(
    "assets/Textures/Atlas/random.png",
    "assets/Textures/Atlas/random.json"
)

local region = atlas:get_region("dogbite")

local player = engine.entity.create()
player:add("transform", { position = Vec2.new(100, 100) })
player:add("sprite", { size = Vec2.new(128, 128), z_index = 10, origin = Vec2.new(0.5, 0.5) })
player:add("texture", { asset_handle = atlas.asset_handle, float_rect = region:get_dimensions() })

local transform = player:get("transform")
local sprite = player:get("sprite")
if not transform then return end
if not sprite then return end

local cam = Camera.new(Vec2.new(640, 360), Vec2.new(1280, 720))
engine.Window:set_camera(cam)

function engine.update(dt)
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

    cam.center = transform.position + transform.position * sprite.origin
    engine.Window:set_camera(cam);
end
