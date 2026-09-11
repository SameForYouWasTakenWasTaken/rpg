#pragma once
#include "EngineContext.hpp"
#include "Rendering/Atlas.hpp"
#include "Types.hpp"

namespace ssg::atlas
{
namespace TexturePacker
{
const Atlas& Load(EngineContext& context, const Filepath& jsonFilepath, TextureHandle textureID);
}
} // namespace ssg::atlas