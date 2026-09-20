#include "FloatRect.hpp"

#include <sol/state.hpp>

#include "SFML/Graphics/Rect.hpp"

namespace ssg::lua::api
{

void register_sf_float_rect(sol::state& state)
{
    state.new_usertype<sf::FloatRect>(
        "FloatRect", sol::call_constructor,
        sol::factories([]() { return sf::FloatRect{}; }, [](float x, float y, float w, float h)
                       { return sf::FloatRect({x, y}, {w, h}); }),

        "x",
        sol::property([](const sf::FloatRect& r) { return r.position.x; },
                      [](sf::FloatRect& r, float v) { r.position.x = v; }),
        "y",
        sol::property([](const sf::FloatRect& r) { return r.position.y; },
                      [](sf::FloatRect& r, float v) { r.position.y = v; }),
        "width",
        sol::property([](const sf::FloatRect& r) { return r.size.x; },
                      [](sf::FloatRect& r, float v) { r.size.x = v; }),
        "height",
        sol::property([](const sf::FloatRect& r) { return r.size.y; },
                      [](sf::FloatRect& r, float v) { r.size.y = v; }),

        "contains", [](const sf::FloatRect& r, float px, float py) { return r.contains({px, py}); },

        "intersects", [](const sf::FloatRect& a, const sf::FloatRect& b)
        { return a.findIntersection(b).has_value(); },

        "intersection",
        [](const sf::FloatRect& a, const sf::FloatRect& b) -> sol::optional<sf::FloatRect>
        {
            auto i = a.findIntersection(b);
            if (!i)
                return sol::nullopt;
            return *i;
        },

        "center",
        [](const sf::FloatRect& r)
        {
            auto c = r.getCenter();
            return std::make_tuple(c.x, c.y); // two return values in Lua
        },

        sol::meta_function::equal_to,
        [](const sf::FloatRect& a, const sf::FloatRect& b) { return a == b; },

        sol::meta_function::to_string,
        [](const sf::FloatRect& r)
        {
            return "FloatRect(" + std::to_string(r.position.x) + ", " +
                   std::to_string(r.position.y) + ", " + std::to_string(r.size.x) + ", " +
                   std::to_string(r.size.y) + ")";
        });
}
} // namespace ssg::lua::api