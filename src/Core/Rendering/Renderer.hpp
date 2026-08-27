#pragma once

#include <limits>

#include "Config/RenderingConfig.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "SFML/Graphics/VertexBuffer.hpp"
#include "Types.hpp"
#include "Window.hpp"
namespace ssg
{

struct RenderObject
{
    Vec2 pos{0, 0};
    Vec2 scale{0, 0};
    Vec2 origin{0, 0}; // Normalized, 0 - 1
    float rotation{0};

    uint8_t zIndex{0};
    sf::Color color{sf::Color::White};

    sf::FloatRect texRect{{0, 0}, {0, 0}};
    const sf::Texture* texture = nullptr;
};

class Renderer
{
  public:
    Renderer();
    ~Renderer() = default;

    Renderer(const Renderer&) = delete;

    Renderer(Renderer&&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    void Submit(const RenderObject& obj);
    void Begin();
    void End(Window& window);

  private:
    void FlushBatch(Window& window, const sf::Texture* texture, std::size_t startVertex,
                    std::size_t endVertex);
    void AppendVertices(const RenderObject& obj);

    sf::VertexArray m_sfVertexArray;
    sf::VertexBuffer m_sfVertexBuffer;

    Array<Vector<RenderObject>, Config::Rendering::Z_INDEX_LAYER_COUNT> m_Layers;
};

} // namespace ssg
