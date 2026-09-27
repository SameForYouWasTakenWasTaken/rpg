#include "ScriptEngine.hpp"

namespace ssg::lua
{

ScriptEngine::ScriptEngine()
{
    m_Lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, sol::lib::table,
                         sol::lib::package);

    // Set the package path to the scripts/ folder, to ensure require() works as intended
    m_Lua["package"]["path"] =
        std::string(m_Lua["package"]["path"]) + ";scripts/?.lua;scripts/?/init.lua";
}
ScriptEngine::~ScriptEngine() {}
bool ScriptEngine::runFile(const Filepath& path)
{
    sol::environment env(m_Lua, sol::create, m_Lua.globals());

    sol::load_result script = m_Lua.load_file(path.string());

    if (!script.valid())
    {
        sol::error err = script;
        std::cerr << "[lua] load error: " << err.what() << '\n';
        return false;
    }

    sol::protected_function function = script;
    sol::set_environment(env, function);

    sol::protected_function_result result = function();

    if (!result.valid())
    {
        sol::error err = result;
        std::cerr << "[lua] runtime error: " << err.what() << '\n';
        return false;
    }

    m_Scripts.emplace_back(path, std::move(env));
    return true;
}
} // namespace ssg::lua