#include "Application.hpp"
#include "Engine.hpp"

int main() 
{
    ssg::Engine::instance().initialize();
    ssg::Application application;
    application.Run();
    return 0;
}
