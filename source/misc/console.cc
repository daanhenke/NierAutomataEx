#include "misc/console.hh"

HANDLE consoleHandle;

void ConsoleInitialize()
{
    AllocConsole();
    SetConsoleTitleA("Nier AutomataEx");

    consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stderr);
    freopen("CONOUT$", "w", stdout);
}

void ConsoleDestroy()
{
    FreeConsole();
}

void ConsoleColor(WORD color)
{
    SetConsoleTextAttribute(consoleHandle, color);
}