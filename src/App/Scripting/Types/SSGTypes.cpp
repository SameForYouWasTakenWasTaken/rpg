#include "SSGTypes.hpp"

#include "Rendering/Camera.hpp"
#include "Rendering/Window.hpp"
#include "SFML/Window/Window.hpp"
#include "Types.hpp"

namespace ssg::lua::api::glob_types
{

void register_vec2(sol::state& state)
{
    auto make =
        sol::factories([]() { return glm::vec2(0.f); }, [](float s) { return glm::vec2(s); },
                       [](float x, float y) { return glm::vec2(x, y); });

    state.new_usertype<glm::vec2>(
        "Vec2", "new", make,

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
void register_camera(sol::state& state)
{

    state.new_usertype<Camera>(
        "Camera", "new", sol::constructors<Camera(), Camera(const Vec2&, const Vec2&)>(),

        "move", &Camera::Move,

        "rotate", &Camera::Rotate,

        "zoom_by", &Camera::Zoom,

        "center",
        sol::property([](const Camera& camera) { return camera.GetCenter(); },
                      [](Camera& camera, const Vec2& size) { camera.SetCenter(size); }),

        "zoom",
        sol::property([](const Camera& camera) { return camera.GetZoom(); },
                      [](Camera& camera, float zoom) { camera.SetZoom(zoom); }),

        "size",
        sol::property([](const Camera& camera) { return camera.GetSize(); },
                      [](Camera& camera, const Vec2& size) { camera.SetSize(size); }),

        "rotation",
        sol::property([](const Camera& camera) { return camera.GetRotation(); },
                      [](Camera& camera, float degrees) { camera.SetRotation(degrees); }));
}
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
void register_window(sol::state& state)
{
    state.new_usertype<Window>(
        "Window", sol::no_constructor, "set_framerate", &Window::SetFramerate,

        "set_size", &Window::SetSize,

        "set_vsync", &Window::SetVSync,

        "set_icon", [](Window& w, std::string_view str) { w.SetIcon(str); },

        "set_title", &Window::SetTitle,

        "is_open", &Window::IsOpen,

        "is_focused", &Window::IsFocused,

        "set_camera",
        [](Window& window, const Camera& camera)
        {
            window.UpdateView(
                [&camera](sf::View& c)
                {
                    auto center = camera.GetCenter();
                    auto size = camera.GetSize();
                    c.setCenter({center.x, center.y});
                    c.setSize({size.x, size.y});
                });
        });
}
void register_all(sol::state& state, sol::table table)
{
    register_vec2(state);
    register_sf_float_rect(state);
    register_camera(state);
    register_window(state);
}
} // namespace ssg::lua::api::glob_types