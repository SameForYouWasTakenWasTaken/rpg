#pragma once

#include <sol/sol.hpp>

#include "Types.hpp"

namespace ssg::lua
{
struct Script
{
    Filepath path;
    sol::environment environment;
};
class ScriptEngine
{
  public:
    ScriptEngine();
    ~ScriptEngine();

    bool runFile(const Filepath& path);
    [[nodiscard]] sol::state& getState() { return m_Lua; }
    [[nodiscard]] std::vector<Script>& getScripts() { return m_Scripts; }

  private:
    sol::state m_Lua;
    std::vector<Script> m_Scripts;
};
} // namespace ssg::lua