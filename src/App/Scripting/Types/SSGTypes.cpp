#include "SSGTypes.hpp"

#include "Types.hpp"

namespace ssg::lua::api::glob_types
{

void register_vec2(sol::state& state)
{
    auto make =
        sol::factories([]() { return glm::vec2(0.f); }, [](float s) { return glm::vec2(s); },
                       [](float x, float y) { return glm::vec2(x, y); });

    state.new_usertype<glm::vec2>(
        "Vec2", "new", make, sol::call_constructor, make,

        "x",
        sol::property([](const glm::vec2& v) { return v.x; },
                      [](glm::vec2& v, float x) { v.x = x; }),
        "y",
        sol::property([](const glm::vec2& v) { return v.y; },
                      [](glm::vec2& v, float y) { v.y = y; }),

        "length", [](const glm::vec2& v) { return glm::length(v); }, "length_squared",
        [](const glm::vec2& v) { return glm::dot(v, v); }, "dot",
        [](const glm::vec2& a, const glm::vec2& b) { return glm::dot(a, b); }, "distance",
        [](const glm::vec2& a, const glm::vec2& b) { return glm::distance(a, b); }, "lerp",
        [](const glm::vec2& a, const glm::vec2& b, float t) { return glm::mix(a, b, t); },
        "normalized",
        [](const glm::vec2& v)
        {
            float len = glm::length(v);
            return len > 0.f ? v / len : glm::vec2(0.f); // glm::normalize would give NaN
        },

        sol::meta_function::addition, [](const glm::vec2& a, const glm::vec2& b) { return a + b; },

        sol::meta_function::subtraction,
        [](const glm::vec2& a, const glm::vec2& b) { return a - b; },

        sol::meta_function::multiplication,
        sol::overload([](const glm::vec2& v, float s) { return v * s; },
                      [](float s, const glm::vec2& v) { return s * v; },
                      [](const glm::vec2& a, const glm::vec2& b)
                      { return a * b; }), // component-wise

        sol::meta_function::division,
        sol::overload([](const glm::vec2& v, float s) { return v / s; },
                      [](const glm::vec2& a, const glm::vec2& b) { return a / b; }),

        sol::meta_function::unary_minus, [](const glm::vec2& v) { return -v; },

        sol::meta_function::equal_to, [](const glm::vec2& a, const glm::vec2& b) { return a == b; },

        sol::meta_function::to_string, [](const glm::vec2& v)
        { return "Vec2(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")"; });
}
void register_all(sol::state& state) { register_vec2(state); }
} // namespace ssg::lua::api::glob_types