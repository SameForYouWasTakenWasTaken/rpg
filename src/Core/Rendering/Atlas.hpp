#pragma once

#include <unordered_map>
#include "Systems/AssetManager.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "Types.hpp"

namespace ssg
{
using SubTextureDimensionList = std::unordered_map<String, sf::FloatRect>;
class Atlas
{
public:
    Atlas() = default;
    ~Atlas() = default;

    Atlas(const Atlas&) = delete;
    Atlas(Atlas&&) = delete;
    Atlas& operator=(Atlas&&) = delete;
    Atlas& operator=(const Atlas&) = delete;

    TextureID LoadTexture(Filepath, Filepath);
    sf::FloatRect GetSubTextureDimensions(const String&);
    const SubTextureDimensionList& GetAllSubTextureDimensions();
private:
    const sf::Texture* m_Texture = nullptr;
    Filepath m_Filepath = "";
    Filepath m_JsonFilepath = "";

    // key: subtexture filename (such as player_walk.jpg)
    // sf::FloatRect: the texture dimensions
    SubTextureDimensionList m_SubTextureDimensions = {};
};
}