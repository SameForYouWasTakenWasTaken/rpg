#include "ScriptEngine.hpp"

namespace ssg::lua
{

ScriptEngine::ScriptEngine()
{
    m_Lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, sol::lib::table);
}
ScriptEngine::~ScriptEngine() {}
bool ScriptEngine::runFile(const Filepath& path)
{
    sol::load_result script = m_Lua.load_file(path.string());
    if (!script.valid())
    {
        sol::error err = script;
        std::cerr << "[lua] load error: " << err.what() << '\n';
        return false;
    }

    sol::protected_function_result result = script();
    if (!result.valid())
    {
        sol::error err = result;
        std::cerr << "[lua] runtime error: " << err.what() << '\n';
        return false;
    }
    return true;
}
} // namespace ssg::lua