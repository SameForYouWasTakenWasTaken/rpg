---@diagnostic disable: need-check-nil
local e = engine.entity.create()
local texture = engine.assets.get_texture("assets/Textures/tree.png")
local assetTextureHandle = texture.asset_handle;
print(assetTextureHandle)
print(assetTextureHandle.id)

print("Texture handle is valid: ", assetTextureHandle:valid(), assetTextureHandle.id)
e:add("transform", { rotation = 45 })
print("transform created")
e:add("texture", { asset_handle = assetTextureHandle, float_rect = FloatRect(0, 0, texture.width, texture.height) })
print("texsture created with width: ", texture.width, " and height: ", texture.height)
e:add("sprite", { size = Vec2(128, 128) })
print("created sprite with size: 128, 128")

local transform = e:get("transform")
transform.position.x = 100;
transform.position.y = 100;
transform.scale = Vec2(5, 5)


print("yayy!!")
