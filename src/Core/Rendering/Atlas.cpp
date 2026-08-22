#include "Atlas.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>

#include "Engine.hpp"
#include "JsonUtil.hpp"
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
    auto& assetManager = Engine::instance().assetManager;

    json::json atlas;
    std::ifstream file(jsonFilepath);

    if (!file.is_open())
        throw std::runtime_error("Could not open atlas .json file: " + jsonFilepath.string());

    file >> atlas;

    if (!json::Has(atlas, "frames"))
        throw std::runtime_error(
            "Atlas .json file is not compatible with GetSubTextureDimensions: " +
            m_JsonFilepath.string());

    const auto& framesArray = json::AccessObjectField(atlas, "frames");

    for (const auto& element : framesArray)
    {
        if (!json::Has(element, "filename"))
            continue;

        if (!json::Has(element, "frame"))
            throw std::runtime_error("No frame in " + jsonFilepath.string());

        const auto& frame = json::AccessObjectField(element, "frame");

        float x = json::AttemptAccessField<float>(frame, "x");
        float y = json::AttemptAccessField<float>(frame, "y");
        float w = json::AttemptAccessField<float>(frame, "w");
        float h = json::AttemptAccessField<float>(frame, "h");

        m_Regions.emplace(json::AttemptAccessField<String>(element, "filename"),
                          sf::FloatRect{{x, y}, {w, h}});
    }

    // LoadTexture() already throw std::runtime, no checks needed
    m_TextureID = texID;
    m_JsonFilepath = jsonFilepath;

    // this ID serves as the "name" of the atlas
    m_ID =
        std::filesystem::path(json::AccessObjectField(atlas, "meta").at("image").get<std::string>())
            .stem()
            .string();
}

sf::FloatRect Atlas::GetRegion(const String& SubTextureFilename)
{
    return m_Regions.at(SubTextureFilename);
}

const RegionList& Atlas::GetAllRegions() { return m_Regions; }

} // namespace ssg