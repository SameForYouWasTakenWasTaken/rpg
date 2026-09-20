#pragma once
#include <typeindex>
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
    std::vector<std::unique_ptr<ISystem>> m_Systems;       // ownership + order
    std::unordered_map<std::type_index, ISystem*> m_Index; // lookup
};
template <System_t T> T& SystemRegistry::Register(std::unique_ptr<T> system)
{
    T& ref = *system;
    [[maybe_unused]] auto [it, inserted] = m_Index.emplace(std::type_index(typeid(T)), &ref);
    assert(inserted && "System type registered twice");

    m_Systems.push_back(std::move(system));
    return ref;
}

template <System_t T> T* SystemRegistry::Find()
{
    auto it = m_Index.find(std::type_index(typeid(T)));
    return it == m_Index.end() ? nullptr : static_cast<T*>(it->second);
}

template <System_t T> T& SystemRegistry::Get()
{
    T* sys = Find<T>();
    assert(sys && "System not registered");
    return *sys;
}
} // namespace ssg