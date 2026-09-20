#pragma once
#include <string_view>
#include <tuple>

#include "Components/CSprite.hpp"
#include "Components/CTexture.hpp"
#include "Components/CTransform.hpp"
#include "ScriptComponent.hpp"

namespace ssg::lua
{
template <> struct ScriptComponent<CTransform>
{
    static constexpr std::string_view name = "transform";
    static constexpr auto fields = std::make_tuple(Field{"position", &CTransform::position},
                                                   Field{"rotation", &CTransform::rotation},
                                                   Field{"scale", &CTransform::scale});
};

template <> struct ScriptComponent<CSprite>
{
    static constexpr std::string_view name = "sprite";
    static constexpr auto fields =
        std::make_tuple(Field{"color", &CSprite::color}, Field{"z_index", &CSprite::zIndex},
                        Field{"origin", &CSprite::origin}, Field{"size", &CSprite::size},
                        Field{"flip_x", &CSprite::flipX}, Field{"flip_y", &CSprite::flipY});
};

template <> struct ScriptComponent<CTexture>
{
    static constexpr std::string_view name = "texture";
    static constexpr auto fields =
        std::make_tuple(Field{"texture_handle", &CTexture::textureHandle},
                        Field{"float_rect", &CTexture::textureRect});
};

using ScriptComponents = TypeList<CTransform, CSprite, CTexture>;

} // namespace ssg::lua