#include "AtlasLoaders.hpp"

#include "Factories/AtlasLoader.hpp"
#include "Systems/AssetManager.hpp"
#include "Types.hpp"

namespace ssg::lua::api::atlas_loaders
{

void register_texturepacker(sol::state& state, sol::table table, EngineContext& context)
{
    table.set_function("load_texture_packer_atlas",
                       [&context](std::string_view texturePath, std::string_view jsonConfigPath)
                       {
                           Filepath texPath{texturePath};
                           Filepath jsonPath{jsonConfigPath};
                           auto texHandle = context.assetManager.LoadTexture(texturePath);
                           return atlas::TexturePacker::Load(context, jsonPath, texHandle);
                       });
}
void register_all(sol::state& state, sol::table table, EngineContext& context)
{
    register_texturepacker(state, table, context);
}
} // namespace ssg::lua::api::atlas_loaders