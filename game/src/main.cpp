#include "DemoApp.h"

#include <cstdlib>
#include <exception>
#include <iostream>

int main()
{
    try
    {
        std::cout
            << "Toetsen: 1/2 effect | pijl omhoog/omlaag niveau | spatie geluid | A audio-backend | Esc stoppen\n";
        game::DemoApp app;
        app.run();
        return EXIT_SUCCESS;
    }
    catch (const std::exception& error)
    {
        std::cerr << "Fout: " << error.what() << '\n';
        return EXIT_FAILURE;
    }
}
