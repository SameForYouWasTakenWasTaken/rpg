#include "Atlas.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>

#include "Engine.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "Types.hpp"

namespace ssg
{

TextureID Atlas::LoadAtlas(const Filepath& jsonFilepath, const Filepath& textureFilepath)
{
    auto& assetManager = Engine::instance().assetManager;

    TextureID texID = assetManager.LoadTexture(textureFilepath);
    LoadAtlas(jsonFilepath, texID);

    return texID;
}

void Atlas::LoadAtlas(const Filepath& jsonFilepath, TextureID texID)
{
    using namespace nlohmann;

    auto& assetManager = Engine::instance().assetManager;

    json atlas;
    std::ifstream file(jsonFilepath);

    if (!file.is_open())
        throw std::runtime_error("Could not open atlas .json file: " + jsonFilepath.string());

    file >> atlas;
    if (!atlas.contains("frames"))
        throw std::runtime_error(
            "Atlas .json file is not compatible with GetSubTextureDimensions: " +
            m_JsonFilepath.string());

    const auto& framesArray = atlas["frames"];
    for (const auto& element : framesArray)
    {
        if (!element.contains("filename"))
            continue;

        if (!element.contains("frame"))
            throw std::runtime_error("No frame in " + jsonFilepath.string());

        const auto& frame = element["frame"];
        float x = static_cast<float>(frame["x"]);
        float y = static_cast<float>(frame["y"]);
        float w = static_cast<float>(frame["w"]);
        float h = static_cast<float>(frame["h"]);

        m_Regions.emplace(element.at("filename").get<String>(), sf::FloatRect{{x, y}, {w, h}});
    }

    // LoadTexture() already throw std::runtime, no checks needed
    m_TextureID = texID;
    m_JsonFilepath = jsonFilepath;

    // this ID serves as the "name" of the atlas
    m_ID = std::filesystem::path(atlas["meta"]["image"].get<std::string>()).stem().string();
}

sf::FloatRect Atlas::GetRegion(const String& SubTextureFilename)
{
    return m_Regions.at(SubTextureFilename);
}

const RegionList& Atlas::GetAllRegions() { return m_Regions; }

} // namespace ssg