#pragma once
#include "Rendering/Window.hpp"

namespace ssg::factory
{
WindowSettings LoadWindowSettings(const Filepath& settingsJsonPath);
}
