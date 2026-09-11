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
    template <typename RenderSink> void AddSink(std::unique_ptr<RenderSink> sink)
    {
        m_Sinks.push_back(std::move(sink));
    }

    template <typename RenderSink> RenderSink* FindSink()
    {
        for (auto& sink : m_Sinks)
        {
            if (auto* result = dynamic_cast<RenderSink*>(sink.get()))
                return result;
        }

        return nullptr;
    }

    template <typename RenderSink> RenderSink& GetSink()
    {
        if (auto* sink = FindSink<RenderSink>())
            return *sink;

        throw std::runtime_error("Requested render sink is not registered!");
    }

    void Begin();
    void End(Window& window);

  private:
    std::vector<std::unique_ptr<rendering::IRenderSink>> m_Sinks;
};

} // namespace ssg
