#include "Precompiled.h"

#include "Application.h"

int main()
{
    Application::Args args = 
    {
        800, 
        800, 
        "Idle Knight" 
    };

    Application app(args);
    app.Run();

    exit(EXIT_SUCCESS);
}

