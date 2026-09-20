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
    void OnAttach(context::SceneContext&) override;
    void OnDetach(context::SceneContext&) override;

    void OnUpdate(float, context::SceneContext& context) override;
    void OnRender(context::SceneContext& context) override;

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