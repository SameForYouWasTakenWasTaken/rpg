#pragma once

#include <nlohmann/json.hpp>

#include "Types.hpp"

namespace ssg::json
{
using nlohmann::json;

template <typename TExpected>
TExpected AttemptAccessField(const nlohmann::json& data, std::string_view field)
{
    auto it = data.find(field);
    if (it == data.end())
        throw std::runtime_error(
            std::format("Missing required field in JSON for entity: {}", std::string(field)));

    try
    {
        return it->get<TExpected>();
    }
    catch (const nlohmann::json::exception& e)
    {
        // Catch type mismatch errors (e.g., field exists, but tried to read a string as int)
        throw std::runtime_error(e.what());
    }
}

bool Has(const nlohmann::json& data, std::string field);

const nlohmann::json& AccessObjectField(const nlohmann::json& data, std::string_view field);

ssg::Vec2 ReadVec2(const nlohmann::json& data, std::string_view field, std::string_view x = "x",
                   std::string_view y = "y");

ssg::Vec3 ReadVec3(const nlohmann::json& data, std::string_view field, std::string_view x = "x",
                   std::string_view y = "y", std::string_view z = "z");

ssg::Vec4 ReadVec4(const nlohmann::json& data, std::string_view field, std::string_view x = "x",
                   std::string_view y = "y", std::string_view z = "z", std::string_view w = "w");
} // namespace ssg::json