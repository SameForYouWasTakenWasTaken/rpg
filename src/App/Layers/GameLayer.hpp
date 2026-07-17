#pragma once

#include "ILayer.hpp"
#include "Rendering/Renderer.hpp"
#include "Types.hpp"

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <memory>

namespace ssg {

class GameLayer final : public ILayer {
public:
    GameLayer() = default;
    ~GameLayer() = default;

    GameLayer(const GameLayer&) = delete;
    GameLayer(GameLayer&&) = delete;
    GameLayer& operator=(const GameLayer&) = delete;
    GameLayer& operator=(GameLayer&&) = delete;

    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate(float dt, ApplicationContext& context) override;
    void OnRender(Renderer& renderer, ApplicationContext& context) override;

private:
    entt::registry m_Registry;
    Vector<std::unique_ptr<sf::Texture>> m_Textures;
};

} // namespace ssg
