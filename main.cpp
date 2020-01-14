#include "App.h"

int main(int argc, char const *argv[])
{
    App app; // allocation sur la pile
    const int err1 = app.ReadOptions(argc, argv);
    if (err1)
    {
        return err1;
    }
    app.Debug();

    const int err2 = app.Run();
    return err2;
}
