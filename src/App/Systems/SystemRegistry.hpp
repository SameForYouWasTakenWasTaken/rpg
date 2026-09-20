#pragma once
#include <unordered_map>

#include "ISystem.hpp"

template <typename T>
concept System_t = std::derived_from<T, ssg::ISystem>;

namespace ssg
{
class SystemRegistry
{
  public:
    SystemRegistry() = default;

    template <System_t T> T& Register(std::unique_ptr<T> system);
    template <System_t T> T& Get();
    template <System_t T> T* Find();

  private:
    std::vector<std::unique_ptr<ISystem>> m_Systems;
};
template <System_t T> T& SystemRegistry::Register(std::unique_ptr<T> system)
{
    m_Systems.push_back(std::move(system));
    return Get<T>();
}
template <System_t T> T& SystemRegistry::Get() { return *Find<T>(); }
template <System_t T> T* SystemRegistry::Find()
{
    for (auto& system : m_Systems)
        if (auto* sys = reinterpret_cast<T*>(system.get()))
            return sys;
    return nullptr;
}
} // namespace ssg