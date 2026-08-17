#include "JsonUtil.hpp"

namespace ssg::json
{

const nlohmann::json& AccessObjectField(const nlohmann::json& data, std::string_view field)
{
    auto it = data.find(field);
    if (it == data.end())
        throw std::runtime_error("Could not access field");

    return *it;
}

bool Has(const nlohmann::json& data, std::string field) { return data.contains(field); }

ssg::Vec2 ReadVec2(const nlohmann::json& data, std::string_view field, std::string_view x,
                   std::string_view y)
{
    const auto& object = AccessObjectField(data, field);

    return {AttemptAccessField<float>(object, x), AttemptAccessField<float>(object, y)};
}

ssg::Vec3 ReadVec3(const nlohmann::json& data, std::string_view field, std::string_view x,
                   std::string_view y, std::string_view z)
{
    const auto& object = AccessObjectField(data, field);

    return {AttemptAccessField<float>(object, x), AttemptAccessField<float>(object, y),
            AttemptAccessField<float>(object, z)};
}

ssg::Vec4 ReadVec4(const nlohmann::json& data, std::string_view field, std::string_view x,
                   std::string_view y, std::string_view z, std::string_view w)
{
    const auto& object = AccessObjectField(data, field);

    return {AttemptAccessField<float>(object, x), AttemptAccessField<float>(object, y),
            AttemptAccessField<float>(object, z), AttemptAccessField<float>(object, w)};
}
} // namespace ssg::json