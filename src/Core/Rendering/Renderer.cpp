#include "Renderer.hpp"

#include <algorithm>
#include <stdexcept>
#include <tuple>

#include "EngineContext.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Transform.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "SFML/System/Angle.hpp"

namespace ssg
{

Renderer::Renderer()
{
    // Make enough room for 1k sprites
    if (!m_sfVertexBuffer.create(
            1'000 * 6)) // 2 triangles, so 3 points * 2 = 6, indexing doesnt work on sfml
        throw std::runtime_error("Unable to preallocate vertex buffer!");
    m_sfVertexBuffer.setPrimitiveType(sf::PrimitiveType::Triangles);
}
void Renderer::Begin()
{
    for (auto& layer : m_Layers)
        layer.clear();

    m_sfVertexArray.clear();
}

void Renderer::End(Window& window)
{
    for (auto& layer : m_Layers)
    {
        if (layer.empty())
            continue;

        // Sort by texture
        std::sort(layer.begin(), layer.end(),
                  [](const auto& a, const auto& b) { return a.texture < b.texture; });

        const sf::Texture* currentTexture = layer.front().texture;

        std::size_t batchStartVertex = m_sfVertexArray.getVertexCount();
        std::size_t batchVertexCount = 0;

        for (const auto& renderObject : layer)
        {
            if (renderObject.texture != currentTexture)
            {
                FlushBatch(window, currentTexture, batchStartVertex, batchVertexCount);

                // Start new batch
                batchStartVertex += batchVertexCount;
                batchVertexCount = 0;
                currentTexture = renderObject.texture;
            }

            // Append vertices
            AppendVertices(renderObject);
            batchVertexCount += 6; // 6 vertices for one quad (2 triangles)
        }

        // Flush final batch
        FlushBatch(window, currentTexture, batchStartVertex, batchVertexCount);
    }
}

void Renderer::AppendVertices(const RenderObject& obj)
{
    sf::Transform transform;
    auto size = obj.scale;
    auto pos = obj.pos;
    auto origin = obj.origin; // ORIGIN IS NORMALIZED!
    auto texRect = obj.texRect;

    transform.translate({pos.x, pos.y});
    transform.rotate(sf::degrees(obj.rotation));
    transform.scale({size.x, size.y});

    // transform points by the origin offset, makes camera centering much simpler
    // and makes sure that the pivot isn't based off of transform.position but by origin.
    // Also helps when flipping X and Y, so the pivot doesn't fuck up
    sf::Vector2f originOffset = {obj.origin.x, obj.origin.y};

    sf::Vector2f p0 = transform.transformPoint({0.0f - originOffset.x, 0.0f - originOffset.y});
    sf::Vector2f p1 = transform.transformPoint({1.0f - originOffset.x, 0.0f - originOffset.y});
    sf::Vector2f p2 = transform.transformPoint({1.0f - originOffset.x, 1.0f - originOffset.y});
    sf::Vector2f p3 = transform.transformPoint({0.0f - originOffset.x, 1.0f - originOffset.y});

    sf::Vector2f t0 = {texRect.position.x, texRect.position.y};
    sf::Vector2f t1 = {texRect.position.x + texRect.size.x, texRect.position.y};
    sf::Vector2f t2 = {texRect.position.x + texRect.size.x, texRect.position.y + texRect.size.y};
    sf::Vector2f t3 = {texRect.position.x, texRect.position.y + texRect.size.y};

    // Triangle 1 (TL -> TR -> BR)
    m_sfVertexArray.append(sf::Vertex{p0, obj.color, t0});
    m_sfVertexArray.append(sf::Vertex{p1, obj.color, t1});
    m_sfVertexArray.append(sf::Vertex{p2, obj.color, t2});

    // Triangle 2 (TL -> BR -> BL)
    m_sfVertexArray.append(sf::Vertex{p0, obj.color, t0});
    m_sfVertexArray.append(sf::Vertex{p2, obj.color, t2});
    m_sfVertexArray.append(sf::Vertex{p3, obj.color, t3});
}

void Renderer::FlushBatch(Window& window, const sf::Texture* texture, std::size_t startVertex,
                          std::size_t endVertex)
{
    assert(m_sfVertexArray.getVertexCount() > 0); // make sure we have at least 1 vertex

    // Resize GPU VBO if needed
    if (m_sfVertexArray.getVertexCount() > m_sfVertexBuffer.getVertexCount())
    {
        // Grow by 1.5x of what we need, so we don't reallocate again next frame
        size_t newCapacity = static_cast<size_t>(m_sfVertexArray.getVertexCount() * 1.5f);
        bool create = m_sfVertexBuffer.create(newCapacity);
        assert(create);
    }

    bool update = m_sfVertexBuffer.update(&m_sfVertexArray[0], m_sfVertexArray.getVertexCount(), 0);
    assert(update);

    sf::RenderStates state;
    state.texture = texture;

    window.Draw(m_sfVertexBuffer, startVertex, endVertex, state);
}

void Renderer::Submit(const RenderObject& obj) { m_Layers[obj.zIndex].push_back(obj); }

} // namespace ssg
