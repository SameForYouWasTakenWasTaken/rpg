#include "GameLayer.hpp"
#include "Application.hpp"
#include "Engine.hpp"
#include "Events/WindowResizeEvent.hpp"
#include "Rendering/Atlas.hpp"
#include "Rendering/Renderer.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "Systems/AssetManager.hpp"

#include "Components/CSprite.hpp"
#include "Components/CTransform.hpp"
#include "Components/CTexture.hpp"

namespace ssg {

void GameLayer::OnAttach() 
{
    Atlas atlas;
    TextureID textureID = atlas.LoadTexture("assets/Textures/Atlas/random.json", "assets/Textures/Atlas/random.png");
    const sf::Texture& tex = Engine::instance().assetManager.GetTexture(textureID);

    sf::FloatRect defaultPNG = atlas.GetSubTextureDimensions("default.png");
    sf::FloatRect dogbite = atlas.GetSubTextureDimensions("dogbite.jpg");
    sf::FloatRect shocked = atlas.GetSubTextureDimensions("shocked.jpg");
    sf::FloatRect uwu     = atlas.GetSubTextureDimensions("uwu.jpg");

    auto makeSquare = [&](float x, float y, float size, TextureID textureID, const sf::FloatRect texRect) {
        entt::entity entity = m_Registry.create();
        auto& sprite = m_Registry.emplace<CSprite>(entity);
        sprite.origin = {0.5, 0.5};

        auto& texture = m_Registry.emplace<CTexture>(entity);
        texture.textureID = textureID;
        texture.textureRect = texRect;
        
        auto& transform = m_Registry.emplace<CTransform>(entity);
        transform.position = {x, y};
        transform.scale = {size, size};

        return entity;
    };

    makeSquare(100.0f, 100.0f, 200.0f, textureID, dogbite);
    makeSquare(340.0f, 100.0f, 200.0f, textureID, dogbite);
    makeSquare(100.0f, 340.0f, 200.0f, textureID, shocked);
    m_LocalPlayer = makeSquare(340.0f, 340.0f, 200.0f, textureID, uwu);

    m_LocalPlayerCamera.SetCenter(m_Registry.get<CTransform>(m_LocalPlayer).position);
    
    // Events
    Engine::instance().eventBus.Sink<WindowResizeEvent>().connect<&GameLayer::OnWindowResize>(this);
}

void GameLayer::OnWindowResize(const WindowResizeEvent& event)
{
    m_LocalPlayerCamera.SetSize({event.Width, event.Height});
}

void GameLayer::OnDetach() 
{
    m_Registry.clear();
}

void GameLayer::OnUpdate(float dt, ApplicationContext& context) 
{
    auto& localPlayerSprite = m_Registry.get<CSprite>(m_LocalPlayer);
    auto& transform = m_Registry.get<CTransform>(m_LocalPlayer);

    constexpr float speed = 300.0f; // pixels per second

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        transform.position.y -= speed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        transform.position.y += speed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        transform.position.x -= speed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        transform.position.x += speed * dt;

    m_LocalPlayerCamera.SetCenter(transform.position);
    context.MainWindow.SetView(m_LocalPlayerCamera.GetView());
}

void GameLayer::OnRender(Renderer& renderer, ApplicationContext& context) 
{
    auto& assetManager = Engine::instance().assetManager;
    auto view = m_Registry.view<CSprite, CTexture, CTransform>();
    for (entt::entity entity : view) 
    {
        auto& sprite = view.get<CSprite>(entity);
        auto& texture = view.get<CTexture>(entity);
        auto& transform = view.get<CTransform>(entity);

        RenderObject obj;
        obj.color = sprite.color;
        obj.origin = sprite.origin;
        obj.zIndex = sprite.zIndex;

        obj.pos = transform.position;
        obj.scale.x = sprite.flipX ? -transform.scale.x : transform.scale.x;
        obj.scale.y = sprite.flipY ? -transform.scale.y : transform.scale.y;
        obj.rotation = transform.rotation;

        obj.texture = &assetManager.GetTexture(texture.textureID);
        obj.texRect = texture.textureRect;

        renderer.Submit(obj);
    }
}

} // namespace ssg
