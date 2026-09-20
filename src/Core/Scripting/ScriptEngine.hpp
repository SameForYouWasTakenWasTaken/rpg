#pragma once

#include <sol/sol.hpp>

#include "Types.hpp"

namespace ssg::lua
{
class ScriptEngine
{
  public:
    ScriptEngine();
    ~ScriptEngine();

    bool runFile(const Filepath& path);
    [[nodiscard]] sol::state& getState() { return m_Lua; }

  private:
    sol::state m_Lua;
};
} // namespace ssg::lua