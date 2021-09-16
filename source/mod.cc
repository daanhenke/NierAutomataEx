#include <Windows.h>
#include "mod.hh"
#include "misc/globals.hh"
#include "misc/console.hh"
#include "misc/directx11.hh"
#include "misc/pattern.hh"

config_t gConfig =
{
    .EntityOverlay = {
        .Enabled = false,
        .MinObjectId = 0x20000,
        .MaxObjectId = 0x30000
    },
    .EnableLevelScaling = true,
};

std::map<std::string, uintptr_t> gGameOffsets = {
    { "Camera", 0x1020870 },
    { "LocalPlayerPtr", 0x1020948},
    { "NextLoadPlayerIndex", 0x11E1C04},
    { "W2S", 0x11553B0}
};

EntityInfoList* gEntityList;
Entity** gPlayerEntityPtr;

void ModMain()
{
    uintptr_t list_accessor = FindPattern("NierAutomata.exe", "74 35 8b c2 c1 f8 08 0f b7 c8 3b 0d") + 12;
    unsigned int reloffset = *reinterpret_cast<unsigned int*>(list_accessor);
    gEntityList = (EntityInfoList*)(list_accessor + reloffset + 4);

    gPlayerEntityPtr = GetGameObject<Entity*>("LocalPlayerPtr");

    while (true)
    {

        Sleep(50);

        if (GetPlayerEntity() == nullptr) continue;

        ApplyEntityMods();
    }
}

void LoadMod()
{
    ConsoleInitialize();
    DXHookStart();

    CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(ModMain), nullptr, 0, nullptr);
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

Entity* GetPlayerEntity()
{
    return *gPlayerEntityPtr;
}