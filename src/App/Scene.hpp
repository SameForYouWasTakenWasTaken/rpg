#pragma once

#include <memory>
#include <entt/entt.hpp>

#include "Application.hpp"
#include "ILayer.hpp"
#include "Rendering/Renderer.hpp"
#include "Types.hpp"

namespace ssg {
    
class IScene {
public:
    entt::registry registry;

    IScene() = default;
    virtual ~IScene() = default;

    IScene(const IScene&) = delete;
    IScene(IScene&&) = delete;
    IScene& operator=(const IScene&) = delete;
    IScene& operator=(IScene&&) = delete;

    virtual void OnUpdate(float, ApplicationContext& context)                = 0;
    virtual void OnRender(Renderer& renderer, ApplicationContext& context)   = 0;

    void PushLayer(std::unique_ptr<ILayer> layer);
    std::unique_ptr<ILayer> PopLayer(std::unique_ptr<ILayer> layer);
    
protected:
    Vector<std::unique_ptr<ILayer>> m_Layers = {};
};

} // namespace ssg
