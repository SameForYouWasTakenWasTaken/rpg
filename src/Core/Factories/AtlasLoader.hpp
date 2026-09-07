#pragma once
#include "EngineContext.hpp"
#include "Rendering/Atlas.hpp"
#include "Types.hpp"

namespace ssg::atlas
{
namespace TexturePacker
{
const Atlas& Load(EngineContext& context, const Filepath& jsonFilepath, TextureID textureID);
}
} // namespace ssg::atlas