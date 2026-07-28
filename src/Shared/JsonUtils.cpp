#include "JsonUtils.hpp"

namespace ssg::json
{
using namespace nlohmann;

const nlohmann::json& AccessObjectField(const nlohmann::json& data, std::string_view field)
{
    auto it = data.find(field);
    if (it == data.end())
        throw std::runtime_error(std::format("Couldn't access field: {}", field));

    return *it;
}

ssg::Vec2 ReadVec2(const nlohmann::json& data, std::string_view field, std::string_view x,
                   std::string_view y)
{
    const auto& object = AccessObjectField(data, field);

    return {AccessField<float>(object, x), AccessField<float>(object, y)};
}

ssg::Vec3 ReadVec3(const nlohmann::json& data, std::string_view field, std::string_view x,
                   std::string_view y, std::string_view z)
{
    const auto& object = AccessObjectField(data, field);

    return {AccessField<float>(object, x), AccessField<float>(object, y),
            AccessField<float>(object, z)};
}

ssg::Vec4 ReadVec4(const nlohmann::json& data, std::string_view field, std::string_view x,
                   std::string_view y, std::string_view z, std::string_view w)
{
    const auto& object = AccessObjectField(data, field);

    return {AccessField<float>(object, x), AccessField<float>(object, y),
            AccessField<float>(object, z), AccessField<float>(object, w)};
}
} // namespace ssg::json