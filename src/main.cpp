#include "utils.h"
#include "Application.h"

int main(int argc, char **argv)
{
    int exitCode = EXIT_SUCCESS;

    OpenLogFile(false);
    CloseLogFile();

    g_pApplication = new Application();

    if (g_pApplication && g_pApplication->Init())
    {
        g_pApplication->Start();
    }
    else
    {
        exitCode = EXIT_FAILURE;
    }

    if (g_pApplication)
    {
        delete g_pApplication;
    }

    return exitCode;
}
