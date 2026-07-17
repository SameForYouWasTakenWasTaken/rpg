#pragma once

#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "SFML/Graphics/VertexBuffer.hpp"
#include "Types.hpp"
#include "Window.hpp"
#include <limits>
namespace ssg {

struct RenderObject
{
    Vec2 pos                        {0, 0};
    Vec2 scale                       {0, 0};
    Vec2 origin                     {0, 0}; // Normalized, 0 - 1
    float rotation                  {0};
    
    uint8_t zIndex                  {0};
    sf::Color color                 {sf::Color::White};
    
    sf::FloatRect texRect           {{0, 0}, {0, 0}};
    const sf::Texture* texture =    nullptr;
};

class Renderer {
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
    void FlushBatch(Window& window, const sf::Texture* texture, std::size_t startVertex, std::size_t endVertex);
    void AppendVertices(const RenderObject& obj);
    
    sf::VertexArray m_sfVertexArray;
    sf::VertexBuffer m_sfVertexBuffer;
        
    static constexpr std::size_t MAX_LAYERS = 
            static_cast<std::size_t>(std::numeric_limits<zIndex_t>::max()) + 1; // +1 cuz indexing starts at 0
    Array<Vector<RenderObject>, MAX_LAYERS> m_Layers;
    
};

} // namespace ssg
