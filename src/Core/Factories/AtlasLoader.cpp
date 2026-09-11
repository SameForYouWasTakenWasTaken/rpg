#include "AtlasLoader.hpp"

#include <fstream>

#include "JsonUtil.hpp"
#include "Systems/AssetManager.hpp"

namespace ssg::atlas
{
namespace TexturePacker
{
const Atlas& Load(EngineContext& context, const Filepath& jsonFilepath, TextureHandle textureID)
{
    json::json atlas;
    std::ifstream file(jsonFilepath);

    if (!file.is_open())
    {
        throw std::runtime_error("Could not open atlas .json file: " + jsonFilepath.string());
    }

    file >> atlas;

    if (!json::Has(atlas, "frames"))
    {
        throw std::runtime_error("Atlas .json file is missing the 'frames' field: " +
                                 jsonFilepath.string());
    }

    RegionList regions;

    const auto& frames = json::AccessObjectField(atlas, "frames");

    for (const auto& element : frames)
    {
        if (!json::Has(element, "filename"))
            continue;

        if (!json::Has(element, "frame"))
        {
            throw std::runtime_error("Atlas frame is missing the 'frame' field: " +
                                     jsonFilepath.string());
        }

        const auto& frame = json::AccessObjectField(element, "frame");

        const auto x = json::AttemptAccessField<float>(frame, "x");
        const auto y = json::AttemptAccessField<float>(frame, "y");
        const auto w = json::AttemptAccessField<float>(frame, "w");
        const auto h = json::AttemptAccessField<float>(frame, "h");

        const auto name = json::AttemptAccessField<String>(element, "filename");

        regions.emplace(name, Region{{x, y}, {w, h}});
    }

    if (!json::Has(atlas, "meta"))
    {
        throw std::runtime_error("Atlas .json file is missing the 'meta' field: " +
                                 jsonFilepath.string());
    }

    const auto& meta = json::AccessObjectField(atlas, "meta");

    if (!json::Has(meta, "image"))
    {
        throw std::runtime_error("Atlas .json file is missing the 'image' field: " +
                                 jsonFilepath.string());
    }

    const auto image = json::AttemptAccessField<String>(meta, "image");

    const auto id = Filepath(image).stem().string();

    context.assetManager.LoadAtlas(Atlas{textureID, id, std::move(regions)});
    return context.assetManager.GetAtlas(id);
}
} // namespace TexturePacker
} // namespace ssg::atlas