#include "ScriptEntity.hpp"

#include "Components/CSprite.hpp"
#include "Components/CTexture.hpp"

namespace ssg::lua
{

void ScriptEntity::Register(sol::state& state)
{
    state.new_usertype<ScriptEntity>("ScriptEntity", "Destroy", &ScriptEntity::Destroy, "IsValid",
                                     &ScriptEntity::IsValid);
}
} // namespace ssg::lua