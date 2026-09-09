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
        !json::Has(data, "framerate"))
        throw std::runtime_error(
            std::format("Did not find title, width, height and (or) framerate instead from file {}",
                        settingsJsonPath.string()));

    auto title = json::AttemptAccessField<String>(data, "title");
    auto width = json::AttemptAccessField<WindowSettings::Size>(data, "width");
    auto height = json::AttemptAccessField<WindowSettings::Size>(data, "height");
    auto framerate = json::AttemptAccessField<WindowSettings::Fps>(data, "framerate");

    return WindowSettings{width, height, framerate, title};
}
} // namespace ssg::factory