#include "Renderer.hpp"
#include "Engine.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Transform.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "SFML/System/Angle.hpp"
#include <algorithm>
#include <stdexcept>

namespace ssg {

Renderer::Renderer()
{
    // Make enough room for 1k sprites
    if (!m_sfVertexBuffer.create(1'000 * 6)) // 2 triangles, so 3 points * 2 = 6, indexing doesnt work on sfml
        throw std::runtime_error("Unable to preallocate vertex buffer!");
    m_sfVertexBuffer.setPrimitiveType(sf::PrimitiveType::Triangles);
    
}
void Renderer::Begin() 
{
    m_sfVertexArray.clear();
    m_RenderObjects.clear();
}

void Renderer::End(Window& window) 
{   
    // Sort by texture
    std::sort(m_RenderObjects.begin(), m_RenderObjects.end(), 
    [](const auto& a, const auto& b){
        return a.texture < b.texture;
    });

    std::size_t idx = 0;
    std::size_t spriteCount = m_RenderObjects.size();
    if (spriteCount == 0) return;
    
    
    const sf::Texture* currentBatchTexture = m_RenderObjects[0].texture;

    size_t batchStartVertex = 0;
    size_t batchVertexCount = 0;

    for (size_t i = 0; i < spriteCount; i++)
    {
        auto& renderObject = m_RenderObjects[i];

        // Will only run after loop reaches next texture
        if (renderObject.texture != currentBatchTexture)
        {
            // Render previous appended vertices
            FlushBatch(window, currentBatchTexture, batchStartVertex, batchVertexCount);

            // Move pointers to track next sub-batch range
            batchStartVertex += batchVertexCount;
            batchVertexCount = 0;
            currentBatchTexture = renderObject.texture;
        }
        
        AppendVertices(renderObject);
        batchVertexCount += 6; // 6 vertices per quad
    }

    FlushBatch(window, currentBatchTexture, batchStartVertex, batchVertexCount);
}

void Renderer::AppendVertices(const RenderObject& obj)
{
    sf::Transform transform;
    auto size = obj.size;
    auto pos = obj.pos;
    auto origin = obj.origin; // ORIGIN IS NORMALIZED!
    auto texRect = obj.texRect;

    transform.translate({pos.x, pos.y});

    transform.rotate(
        sf::degrees(obj.rotation), 
        {size.x * origin.x, size.y * origin.y}
    );

    transform.scale({size.x, size.y});

    sf::Vector2f p0 = transform.transformPoint({0.0f, 0.0f}); // TL
    sf::Vector2f p1 = transform.transformPoint({1.0f, 0.0f}); // TR
    sf::Vector2f p2 = transform.transformPoint({1.0f, 1.0f}); // BR
    sf::Vector2f p3 = transform.transformPoint({0.0f, 1.0f}); // BL

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

void Renderer::FlushBatch(Window& window, const sf::Texture* texture, std::size_t startVertex, std::size_t endVertex)
{
    assert(m_sfVertexArray.getVertexCount() > 0); // make sure we have at least 1 vertex

    // Resize GPU VBO if needed
    if (m_sfVertexArray.getVertexCount() > m_sfVertexBuffer.getVertexCount()) {
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

void Renderer::Submit(const RenderObject& object)
{
    m_RenderObjects.push_back(object);
}

} // namespace ssg
