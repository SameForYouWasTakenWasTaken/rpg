#include "AssetManager.hpp"

#include <memory>

namespace ssg
{
TextureID AssetManager::LoadTexture(Filepath path)
{
    if (m_Filepaths.find(path) != m_Filepaths.end())
        return m_Filepaths[path]; // Texture is already loaded

    auto texture = std::make_unique<sf::Texture>(path);
    if (!texture)
        throw std::runtime_error("Could not get texture of path " + path.string() + "!");

    m_Textures.emplace(counter, std::move(texture));
    m_Filepaths.emplace(path, counter);

    return counter++;
}

const sf::Texture& AssetManager::GetTexture(TextureID id) { return *(m_Textures[id]); }
const sf::Texture& AssetManager::GetTexture(Filepath path)
{
    TextureID id = m_Filepaths[path];
    return GetTexture(id);
}
} // namespace ssg