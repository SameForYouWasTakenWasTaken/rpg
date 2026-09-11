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
void Renderer::Begin() const
{
    for (auto& sink : m_Sinks)
        sink->Begin();
}

void Renderer::ForwardEvent(const sf::Event& e) const
{
    for (auto& sink : m_Sinks)
        sink->HandleEvents(e);
}

void Renderer::End(Window& window) const
{
    for (auto& sink : m_Sinks)
        sink->End(window);
}

} // namespace ssg
