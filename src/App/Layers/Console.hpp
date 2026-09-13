#pragma once
#include "ILayer.hpp"
#include "Logger.hpp"
#include "imgui.h"

namespace ssg
{
class Console : public ILayer
{
  public:
    explicit Console(EngineContext& m_engine_context) : m_EngineContext(m_engine_context) {}
    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate(float, ApplicationContext& context) override;
    void OnRender(Renderer& renderer, ApplicationContext& context) override;

  private:
    static ImVec4 GetLogLevelColor(log::LogLevel logLevel);
    static const char* GetLogLevelPrefix(log::LogLevel level);
    static void ExecuteCommand(const std::string& command, EngineContext& context);

    std::array<char, 256> m_InputBuffer{};
    std::array<char, 64> m_FilterBuffer{};
    bool m_AutoScroll{true};

    EngineContext& m_EngineContext;
};
} // namespace ssg