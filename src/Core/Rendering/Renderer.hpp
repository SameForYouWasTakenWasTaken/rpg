#pragma once

#include <limits>

#include "Config/RenderingConfig.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "SFML/Graphics/VertexBuffer.hpp"
#include "Sinks/IRenderSink.hpp"
#include "Types.hpp"
#include "Window.hpp"
namespace ssg
{
template <typename T>
concept RenderSink = std::derived_from<T, rendering::IRenderSink>;

class Renderer
{
  public:
    Renderer();
    ~Renderer() = default;

    Renderer(const Renderer&) = delete;

    Renderer(Renderer&&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;
    template <RenderSink T> void AddSink(std::unique_ptr<T> sink)
    {
        m_Sinks.push_back(std::move(sink));
    }

    template <RenderSink T> T* FindSink()
    {
        for (auto& sink : m_Sinks)
        {
            if (auto* result = dynamic_cast<T*>(sink.get()))
                return result;
        }

        return nullptr;
    }

    template <RenderSink T> T& GetSink()
    {
        if (auto* sink = FindSink<T>())
            return *sink;

        throw std::runtime_error("Requested render sink is not registered!");
    }

    void Begin();
    void ForwardEvent(const sf::Event& e);
    void End(Window& window);

  private:
    std::vector<std::unique_ptr<rendering::IRenderSink>> m_Sinks;
};

} // namespace ssg
