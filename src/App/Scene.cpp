#include "Scene.hpp"
#include <memory>

namespace ssg {
void IScene::PushLayer(std::unique_ptr<ILayer> layer)
{
    m_Layers.push_back(std::move(layer));
    m_Layers.back()->OnAttach();
}

std::unique_ptr<ILayer> IScene::PopLayer(std::unique_ptr<ILayer> layer)
{
    if (m_Layers.empty()) return nullptr;

    std::unique_ptr<ILayer> popped = std::move(m_Layers.back());
    
    m_Layers.pop_back();
    
    popped->OnDetach();

    return popped;
}
} // namespace ssg
