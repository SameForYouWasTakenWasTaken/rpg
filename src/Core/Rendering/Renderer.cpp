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
}
void Renderer::Begin()
{
    for (auto& sink : m_Sinks)
    {
        sink->Begin();
    }
}

void Renderer::End(Window& window)
{
    for (auto& sink : m_Sinks)
    {
        sink->End(window);
    }
}

} // namespace ssg
