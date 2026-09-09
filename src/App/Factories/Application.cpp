#include "Application.hpp"

#include <fstream>

#include "JsonUtil.hpp"

namespace ssg::factory
{

WindowSettings LoadWindowSettings(const Filepath& settingsJsonPath)
{
    std::ifstream file(settingsJsonPath);
    if (!file.is_open())
        throw std::runtime_error(std::format("Couldn't open file {} !", settingsJsonPath.string()));

    json::json data;
    file >> data;

    if (!json::Has(data, "title") || !json::Has(data, "width") || !json::Has(data, "height") ||
        !json::Has(data, "framerate") || !json::Has(data, "vsync"))
        throw std::runtime_error(
            std::format("Did not find title, width, height, vsync and (or) framerate from file {}",
                        settingsJsonPath.string()));

    auto title = json::AttemptAccessField<String>(data, "title");
    auto width = json::AttemptAccessField<WindowSettings::Size>(data, "width");
    auto height = json::AttemptAccessField<WindowSettings::Size>(data, "height");
    auto framerate = json::AttemptAccessField<WindowSettings::Fps>(data, "framerate");
    auto vsync = json::AttemptAccessField<bool>(data, "vsync");

    // optional settings that don't need strict checks, and can be passed in as "null" or default values
    Filepath iconFilepath{};

    if (json::Has(data, "icon"))
        iconFilepath = json::AttemptAccessField<Filepath>(data, "icon");

    return WindowSettings{width, height, framerate, title, vsync, iconFilepath};
}
} // namespace ssg::factory