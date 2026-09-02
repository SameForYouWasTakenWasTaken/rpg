#include "Application.hpp"
#include "Engine.hpp"
#include "EngineContext.hpp"

int main()
{
    ssg::Engine engine;
    engine.initialize();

    ssg::EngineContext context{engine};
    ssg::Application application{context};

    application.Run();
    return 0;
}
