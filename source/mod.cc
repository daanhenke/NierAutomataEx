#include <Windows.h>
#include "mod.hh"
#include "misc/globals.hh"
#include "misc/console.hh"
#include "misc/directx11.hh"

std::map<std::string, uintptr_t> gGameOffsets = {
    { "Camera", 0x1020870 },
    { "LocalPlayerPtr", 0x1020870 + 0xA8 },
    { "EntityList", 0x1029510 }
};

void LoadMod()
{
    ConsoleInitialize();
    DXHookStart();
}

void UnloadModInner()
{
    MessageBoxA(nullptr, "Uninjecting is not implemented :D", "Uninjecting", MB_OK);

    DXHookStop();
    ConsoleDestroy();

    FreeLibraryAndExitThread(gModuleHandle, 0);
}

void UnloadMod()
{
    CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(UnloadModInner), nullptr, 0, nullptr);
}