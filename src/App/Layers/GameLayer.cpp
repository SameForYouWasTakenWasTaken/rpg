#include "GameLayer.hpp"
#include "Application.hpp"
#include "Engine.hpp"
#include "Rendering/Atlas.hpp"
#include "Rendering/Renderer.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "Systems/AssetManager.hpp"

namespace ssg {

void GameLayer::OnAttach() 
{
    Atlas atlas;
    TextureID textureID = atlas.LoadTexture("assets/Textures/Atlas/random.json", "assets/Textures/Atlas/random.png");
    const sf::Texture& texture = Engine::instance().assetManager.GetTexture(textureID);

    sf::FloatRect defaultPNG = atlas.GetSubTextureDimensions("default.png");
    sf::FloatRect dogbite = atlas.GetSubTextureDimensions("dogbite.jpg");
    sf::FloatRect shocked = atlas.GetSubTextureDimensions("shocked.jpg");
    sf::FloatRect uwu     = atlas.GetSubTextureDimensions("uwu.jpg");

    auto makeSquare = [&](float x, float y, float size, const sf::FloatRect tex) {
        entt::entity entity = m_Registry.create();

        auto& object = m_Registry.emplace<RenderObject>(entity);
        object.pos    = {x, y};
        object.size   = {size, size};
        object.origin = {0.0f, 0.0f};
        object.color  = sf::Color::White;
        object.texRect = tex;
        object.texture = &texture;
    };

    makeSquare(100.0f, 100.0f, 200.0f, dogbite);
    makeSquare(340.0f, 100.0f, 200.0f, dogbite);
    makeSquare(100.0f, 340.0f, 200.0f, shocked);
    makeSquare(340.0f, 340.0f, 200.0f, uwu);
}

void GameLayer::OnDetach() 
{
    m_Registry.clear();
    m_Textures.clear();
}

void GameLayer::OnUpdate(float dt, ApplicationContext& context) 
{
    
}

void GameLayer::OnRender(Renderer& renderer, ApplicationContext& context) 
{

    auto view = m_Registry.view<RenderObject>();
    for (entt::entity entity : view) 
    {
        renderer.Submit(view.get<RenderObject>(entity));
    }
}

} // namespace ssg
