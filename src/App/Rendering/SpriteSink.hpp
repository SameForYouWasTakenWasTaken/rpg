#pragma once
#include "Config/RenderingConfig.hpp"
#include "Rendering/Sinks/IRenderSink.hpp"
#include "SFML/Graphics.hpp"
#include "Types.hpp"

namespace ssg::rendering
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

class SpriteSink : public IRenderSink
{
  public:
    SpriteSink();

    void Begin() override;
    void Submit(const RenderObject& object);
    void End(Window& window) override;

  private:
    void FlushBatch(Window& window, const sf::Texture* texture, std::size_t startVertex,
                    std::size_t endVertex);
    void AppendVertices(const RenderObject& obj);
    sf::VertexArray m_sfVertexArray;
    sf::VertexBuffer m_sfVertexBuffer;
    Array<Vector<RenderObject>, Config::Rendering::Z_INDEX_LAYER_COUNT> m_Layers;
};
} // namespace ssg::rendering