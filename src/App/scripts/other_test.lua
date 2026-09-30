---@diagnostic disable: need-check-nil
local input = engine.input
local e = engine.entity.create()

local atlas = engine.assets.load_texture_packer_atlas(
	"assets/Textures/Atlas/random.png",
	"assets/Textures/Atlas/random.json")

local region = atlas:get_region("dogbite") -- functions nearly identically as a texture
local assetTextureHandle = atlas.asset_handle

print(assetTextureHandle)
print(assetTextureHandle.id)

print("Texture handle is valid: ", assetTextureHandle:valid(), assetTextureHandle.id)
e:add("transform", { rotation = 45 })
print("transform created")
e:add("texture",
	{
		asset_handle = assetTextureHandle,
		float_rect = region:get_dimensions(),
	})
print("texsture created with width: ", region.size.x, " and height: ", region.size.y)
e:add("sprite", { size = Vec2.new(128, 128) })
print("created sprite with size: 128, 128")

local transform = e:get("transform")
transform.position.x = 100
transform.position.y = 100
transform.scale = Vec2.new(5, 5)

local window = engine.Window
window:set_framerate(60)
window:set_title("Gooner")

print(window:is_open())

for _, e in pairs(engine.entity.get_all_of("transform")) do
	print(e)
end

function engine.update(dt)
	if input.is_key_down(input.Key.W) then
		print("W is held")
	end
end

input.key_pressed:Connect(function(event)
	print("key " .. event.key .. " was pressed")

	if (event.key == input.Key.F) then

	end
end)

print("yayy!!")
