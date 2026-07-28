#pragma once
#include <format>
#include <nlohmann/json.hpp>

#include "Types.hpp"

namespace ssg::json
{
using namespace nlohmann;
template <typename TExpected>
TExpected AccessField(const nlohmann::json& data, std::string_view field)
{
    auto it = data.find(field);
    if (it == data.end())
    {
        throw std::runtime_error(std::format("Missing required field in JSON: {}", field));
    }

    try
    {
        return it->get<TExpected>();
    }
    catch (const nlohmann::json::exception& e)
    {
        // Catch type mismatch errors (e.g., field exists, but tried to read a string as int)
        throw std::runtime_error(std::format("Type mismatch for field {}, :{}", field, e.what()));
    }
}
const nlohmann::json& AccessObjectField(const nlohmann::json& data, std::string_view field);

ssg::Vec2 ReadVec2(const nlohmann::json& data, std::string_view field, std::string_view x = "x",
                   std::string_view y = "y");

ssg::Vec3 ReadVec3(const nlohmann::json& data, std::string_view field, std::string_view x = "x",
                   std::string_view y = "y", std::string_view z = "z");

ssg::Vec4 ReadVec4(const nlohmann::json& data, std::string_view field, std::string_view x = "x",
                   std::string_view y = "y", std::string_view z = "z", std::string_view w = "w");
} // namespace ssg::json