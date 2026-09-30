#pragma once
#include "Rendering/Window.hpp"

namespace ssg
{
struct ApplicationContext
{
    ApplicationContext(const ApplicationContext&) = delete;
    ApplicationContext(ApplicationContext&&) = delete;
    ApplicationContext& operator=(const ApplicationContext&) = delete;
    ApplicationContext& operator=(ApplicationContext&&) = delete;

    ApplicationContext(Window& w) : MainWindow(w) {}

    Window& MainWindow;
};

} // namespace ssg
