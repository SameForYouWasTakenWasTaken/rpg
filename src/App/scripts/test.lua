local textureHandle = asset_manager.get_texture_handle("assets/Textures/dogbite.jpg")
print("texture handle:", textureHandle)

local entity = Entity.create()
print("Entity:", entity)

local sprite = entity:GetSprite()
sprite.size = Vec2(128, 128)
print("Sprite:", sprite)

local texture = entity:GetTexture()
texture.textureID = textureHandle
texture.textureRect = FloatRect(0, 0, 32, 32)
print("Texture:", texture)