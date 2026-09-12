#include "ImGuiCaptureProvider.hpp"

#include "imgui.h"

namespace ssg
{

bool ImGuiCaptureProvider::WantsCaptureMouse() const { return ImGui::GetIO().WantCaptureMouse; }
bool ImGuiCaptureProvider::WantsCaptureKeyboard() const
{
    return ImGui::GetIO().WantCaptureKeyboard;
}
} // namespace ssg