#pragma once
#include "EngineContext.hpp"

#include "Engine.hpp"

namespace ssg
{
EngineContext::EngineContext(Engine& engine)
    : engine(engine), logger(engine.GetLogger()), assetManager(engine.GetAssetManager())
{
}
} // namespace ssg