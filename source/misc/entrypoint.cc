#include <Windows.h>

#include "mod.hh"
#include "misc/globals.hh"

class EntryPoint
{
public:
    EntryPoint()
    {
        DisableThreadLibraryCalls(gModuleHandle);
        CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(LoadMod), nullptr, 0, nullptr);
    }

    ~EntryPoint()
    {
        UnloadMod();
    }
};

EntryPoint _ep;