#include "GameLayer.hpp"

#include "Application.hpp"
#include "Components/CSprite.hpp"
#include "Components/CTexture.hpp"
#include "Components/CTransform.hpp"
#include "Components/CWorldTransform.hpp"
#include "Engine.hpp"
#include "Events/WindowResizeEvent.hpp"
#include "Factories/Default.hpp"
#include "Rendering/Atlas.hpp"
#include "Rendering/Renderer.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "Systems/AssetManager.hpp"
#include "Systems/Hierarchy.hpp"

namespace ssg
{

void GameLayer::OnAttach()
{
    auto& assetManager = Engine::instance().assetManager;
    auto atlasID = assetManager.LoadAtlas("assets/Textures/Atlas/random.json",
                                          "assets/Textures/Atlas/random.png");
    auto& atlas = assetManager.GetAtlas(atlasID);
    auto textureID = atlas.GetTextureID();

    Region defaultPNG = atlas.GetRegion("default");
    Region dogbite = atlas.GetRegion("dogbite");
    Region shocked = atlas.GetRegion("shocked");
    Region uwu = atlas.GetRegion("uwu");

    auto makeSquare = [&](float x, float y, float size, TextureID textureID, const Region texRect)
    {
        entt::entity entity = m_Registry.create();
        factory::AddDefaultSprite(m_Registry, entity, textureID, texRect, {size, size});

        // modify local transform (scale stays a {1,1} multiplier; size lives on the sprite)
        auto& transform = m_Registry.get<CTransform>(entity);
        transform.position = {x, y};

        return entity;
    };

    makeSquare(100.0f, 100.0f, 200.0f, textureID, dogbite);
    makeSquare(340.0f, 100.0f, 200.0f, textureID, dogbite);
    makeSquare(100.0f, 340.0f, 200.0f, textureID, shocked);
    makeSquare(340.0f, 340.0f, 200.0f, textureID, uwu);

    auto player = assetManager.GetEntityDefinition("data/characters/player.json");
    auto& playerAtlas = assetManager.GetAtlas(player.atlasID);
    auto& playerRegion = player.region;
    m_LocalPlayer = makeSquare(500.f, 500.f, 200.f, playerAtlas.GetTextureID(), playerRegion);
    m_LocalPlayerCamera.SetCenter(m_Registry.get<CTransform>(m_LocalPlayer).position);

    // World transforms must be current for KeepWorld rebasing to be correct.
    m_TransformSystem.Update(0.0f);
    hierarchy::AttachChild(m_Registry, m_LocalPlayer, other, hierarchy::AttachMode::KeepWorld);

    // Events
    Engine::instance().eventBus.Sink<WindowResizeEvent>().connect<&GameLayer::OnWindowResize>(this);
}

void GameLayer::OnWindowResize(const WindowResizeEvent& event)
{
    m_LocalPlayerCamera.SetSize({event.Width, event.Height});
}

void GameLayer::OnDetach() { m_Registry.clear(); }

void GameLayer::OnUpdate(float dt, ApplicationContext& context)
{
    // 1. Gameplay writes LOCAL transforms.
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

    // 2. Derive WORLD transforms from local + hierarchy.
    m_TransformSystem.Update(dt);

    // 3. Spatial grid now sees up-to-date positions.
    m_SpatialGrid.Rebuild();

    // 4. Camera follows the player's world position.
    m_LocalPlayerCamera.SetCenter(m_Registry.get<CWorldTransform>(m_LocalPlayer).position);
    context.MainWindow.SetView(m_LocalPlayerCamera.GetView());
}

void GameLayer::OnRender(Renderer& renderer, ApplicationContext& context)
{
    auto& assetManager = Engine::instance().assetManager;
    auto view = m_Registry.view<CSprite, CTexture, CWorldTransform>();
    for (entt::entity entity : view)
    {
        auto& sprite = view.get<CSprite>(entity);
        auto& texture = view.get<CTexture>(entity);
        auto& transform = view.get<CWorldTransform>(entity);

        RenderObject obj;
        obj.color = sprite.color;
        obj.origin = sprite.origin;
        obj.zIndex = sprite.zIndex;

        obj.pos = transform.position;

        // Final pixel size = sprite size * world scale multiplier.
        const Vec2 finalSize = sprite.size * transform.scale;
        obj.scale.x = sprite.flipX ? -finalSize.x : finalSize.x;
        obj.scale.y = sprite.flipY ? -finalSize.y : finalSize.y;
        obj.rotation = transform.rotation;

        obj.texture = &assetManager.GetTexture(texture.textureID);
        obj.texRect = texture.textureRect;

        renderer.Submit(obj);
    }
}

} // namespace ssg
