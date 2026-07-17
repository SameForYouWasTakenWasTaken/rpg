#include <fstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

#include "Atlas.hpp"
#include "Engine.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "Types.hpp"


namespace ssg
{

TextureID Atlas::LoadTexture(Filepath jsonFilepath, Filepath textureFilepath)
{
    using namespace nlohmann;

    auto& assetManager =  Engine::instance().assetManager;

    json atlas;
    std::ifstream file(jsonFilepath);

    if (!file.is_open())
        throw std::runtime_error("Could not open atlas .json file: " + jsonFilepath.string());

    file >> atlas;
    if (!atlas.contains("frames"))
        throw std::runtime_error("Atlas .json file is not compatible with GetSubTextureDimensions: " + m_JsonFilepath.string());
    
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

        m_SubTextureDimensions.emplace(
            element.at("filename").get<String>(), 
            sf::FloatRect{{x, y}, {w, h}}
        );
    }

    // LoadTexture() already throw std::runtime, no checks needed
    TextureID id = assetManager.LoadTexture(textureFilepath);
    m_Texture = &assetManager.GetTexture(id);
    m_Filepath = textureFilepath;
    m_JsonFilepath = jsonFilepath;

    return id;
}

sf::FloatRect Atlas::GetSubTextureDimensions(const String& SubTextureFilename)
{
    return m_SubTextureDimensions.at(SubTextureFilename);
}

const SubTextureDimensionList& Atlas::GetAllSubTextureDimensions()
{
    return m_SubTextureDimensions;
}

}