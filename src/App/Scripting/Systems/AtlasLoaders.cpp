#include "AtlasLoaders.hpp"

#include "Factories/AtlasLoader.hpp"
#include "Systems/AssetManager.hpp"
#include "Types.hpp"

namespace ssg::lua::api::atlas_loaders
{

void register_texturepacker(LuaContext& lua_context)
{
    auto table = lua_context.namespace_table["assets"].get<sol::table>();
    auto& context = lua_context.engine_context;
    table.set_function("load_texture_packer_atlas",
                       [&context](std::string_view texturePath, std::string_view jsonConfigPath)
                       {
                           Filepath texPath{texturePath};
                           Filepath jsonPath{jsonConfigPath};
                           auto texHandle = context.assetManager.LoadTexture(texturePath);
                           return atlas::TexturePacker::Load(context, jsonPath, texHandle);
                       });
}
void register_all(LuaContext& lua_context)
{
    register_texturepacker(lua_context);
}
} // namespace ssg::lua::api::atlas_loaders
