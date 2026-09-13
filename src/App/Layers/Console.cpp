#include "Console.hpp"

#include "imgui.h"

namespace ssg
{

void Console::OnAttach() {}
void Console::OnDetach() {}
void Console::OnUpdate(float, ApplicationContext& context) {}
void Console::OnRender(Renderer& renderer, ApplicationContext& applicationContext)
{

    ImGui::SetNextWindowSize(ImVec2(600, 350), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Console"))
    {
        ImGui::End();
        return;
    }

    auto* memorySink = m_EngineContext.logger.FindSink<log::MemorySink>();
    if (!memorySink)
    {
        ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f),
                           "Error: No MemorySink attached to Logger!");
        ImGui::End();
        return;
    }

    // --- Top Bar: Controls & Filters ---
    if (ImGui::Button("Clear"))
    {
        memorySink->entries.clear();
    }
    ImGui::SameLine();
    bool copyToClipboard = ImGui::Button("Copy");
    ImGui::SameLine();
    ImGui::Checkbox("Auto-scroll", &m_AutoScroll);
    ImGui::SameLine();

    // Category filter text box
    ImGui::SetNextItemWidth(150.0f);
    ImGui::InputText("Filter", m_FilterBuffer.data(), m_FilterBuffer.size());

    ImGui::Separator();

    // --- Log Output Window ---
    const float footerHeight = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    if (ImGui::BeginChild("LogRegion", ImVec2(0, -footerHeight), false,
                          ImGuiWindowFlags_HorizontalScrollbar))
    {
        if (copyToClipboard)
        {
            ImGui::LogToClipboard();
        }

        std::string filterStr(m_FilterBuffer.data());

        for (const auto& entry : memorySink->entries)
        {
            // Apply category/message filter if present
            if (!filterStr.empty())
            {
                if (entry.category.find(filterStr) == std::string::npos &&
                    entry.message.find(filterStr) == std::string::npos)
                {
                    continue;
                }
            }

            // Colorize based on LogLevel
            ImVec4 color = GetLogLevelColor(entry.level);
            ImGui::PushStyleColor(ImGuiCol_Text, color);

            // Format: > [CATEGORY] [SOURCE] Message
            ImGui::Text("> [%s] [%s] %s", entry.category.c_str(), GetLogLevelPrefix(entry.level),
                        entry.message.c_str());

            ImGui::PopStyleColor();
        }

        if (copyToClipboard)
        {
            ImGui::LogFinish();
        }

        // Auto-scroll to bottom on new log entries
        if (m_AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        {
            ImGui::SetScrollHereY(1.0f);
        }
    }
    ImGui::EndChild();

    ImGui::Separator();

    // --- Bottom Bar: Command Input ---
    bool reclaimFocus = false;
    ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;

    ImGui::SetNextItemWidth(-1.0f); // Stretch to right edge
    if (ImGui::InputText("##ConsoleInput", m_InputBuffer.data(), m_InputBuffer.size(), flags))
    {
        std::string command(m_InputBuffer.data());
        if (!command.empty())
        {
            ExecuteCommand(command, m_EngineContext);
            m_InputBuffer.fill(0);
        }
        reclaimFocus = true;
    }

    // Keep input focused when entering commands
    ImGui::SetItemDefaultFocus();
    if (reclaimFocus)
    {
        ImGui::SetKeyboardFocusHere(-1);
    }

    ImGui::End();
}
ImVec4 Console::GetLogLevelColor(log::LogLevel logLevel)
{
    switch (logLevel)
    {
    case log::LogLevel::Trace:
        return ImVec4(0.6f, 0.6f, 0.6f, 1.0f); // Gray
    case log::LogLevel::Debug:
        return ImVec4(0.4f, 0.8f, 1.0f, 1.0f); // Light Blue
    case log::LogLevel::Info:
        return ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // White
    case log::LogLevel::Warn:
        return ImVec4(1.0f, 0.8f, 0.2f, 1.0f); // Yellow
    case log::LogLevel::Error:
        return ImVec4(1.0f, 0.3f, 0.3f, 1.0f); // Red
    case log::LogLevel::Fatal:
        return ImVec4(1.0f, 0.1f, 0.1f, 1.0f); // Bright Red
    default:
        return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
}
const char* Console::GetLogLevelPrefix(log::LogLevel level)
{
    switch (level)
    {
    case log::LogLevel::Trace:
        return "TRACE";
    case log::LogLevel::Debug:
        return "DEBUG";
    case log::LogLevel::Info:
        return "INFO";
    case log::LogLevel::Warn:
        return "WARN";
    case log::LogLevel::Error:
        return "ERROR";
    case log::LogLevel::Fatal:
        return "FATAL";
    default:
        return "LOG";
    }
}
void Console::ExecuteCommand(const std::string& command, EngineContext& context)
{
    // Log the user's input directly back to the logger
    context.logger.Info("Console", "{}", command);

    if (command == "clear")
    {
        if (auto* sink = context.logger.FindSink<log::MemorySink>())
        {
            sink->entries.clear();
        }
    }
    else if (command == "help")
    {
        context.logger.Info("Console", "Available commands: help, clear, fps");
    }
    else
    {
        context.logger.Warn("Console", "Unknown command: {}", command);
    }
}

} // namespace ssg