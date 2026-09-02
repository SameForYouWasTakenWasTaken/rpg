#pragma once

namespace ssg
{
class AssetManager;
namespace log
{
class Logger;
}
struct Engine;
struct EngineContext
{
    Engine& engine;

    // quick access variables
    log::Logger& logger;
    AssetManager& assetManager;

    EngineContext(Engine& engine);
};
} // namespace ssg