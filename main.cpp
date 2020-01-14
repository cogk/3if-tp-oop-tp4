#include "App.h"

int main(int argc, char const *argv[])
{
    App app(argc, argv); // allocation sur la pile
    const int result = app.Run();
    return result;
}
