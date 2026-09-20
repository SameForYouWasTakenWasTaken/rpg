local e = engine.entity.create()
local textureHandle = engine.assets.get_texture_handle("assets/Textures/tree.png");
local texture = engine.assets.get_texture(textureHandle);

print("Texture handle is valid: ", textureHandle:valid(), textureHandle.id)
e:add("transform", { rotation = 45 })
print("transform created")
e:add("texture", { texture_handle = textureHandle, float_rect = FloatRect(0, 0, texture.width, texture.height) })
print("texsture created with width: ", texture.width, " and height: ", texture.height)
e:add("sprite", { size = Vec2(128, 128) })
print("created sprite with size: 128, 128")

local transform = e:get("transform")
transform.position = Vec2(100, 100)
transform.scale = Vec2(5, 5)

if transform then
    print(transform.position)
    print(transform.scale)
end


print("yayy!!")
