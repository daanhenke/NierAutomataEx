#include <Windows.h>
#include "mod.hh"
#include "misc/globals.hh"
#include "misc/console.hh"
#include "misc/directx11.hh"
#include "misc/pattern.hh"
#include "automata/entities.hh"

#include "MinHook.h"

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
    { "W2S", 0x11553B0},
    { "Entity_SetMaxHealth", 0x4FDB50}
};

EntityInfoList* gEntityList;
Entity** gPlayerEntityPtr;

typedef void (__fastcall* EntityCreateFunc_t)(void* entity, int hp);
typedef Entity* (__fastcall* SceneEntitySystem_SpawnEntity_t)(void* _this, const char* name, unsigned int id, void* something);
SceneEntitySystem_SpawnEntity_t ogEntityCreateFunc = nullptr;

void OnNewEntity(Entity* entity);
Entity* __fastcall HookedEntityCreateFunc(void* _this, const char* name, int id, void* something)
{
    //ConsoleWriteColor(FOREGROUND_RED, "HookedEntityCreateFunc called: %s %x %llx\n", name, id, something);

    if (id == 0x33000 || (unsigned int)id == 0x33000)
    {
        ConsoleWrite("Found pod spawn: %x, %s\n", id, name);
        ConsoleWrite("Create_t ptr: %llx\n", something);
    }

    Entity* ent = ogEntityCreateFunc(_this, name, id, something);
    OnNewEntity(ent);
    return ent;
}

void ModMain()
{
    if (MH_Initialize() != MH_OK)
    {
        ConsoleWriteColor(FOREGROUND_RED, "Failed to initialize minhook\n");
        return;
    }

    char* hNier = (char*)GetModuleHandleA("NierAutomata.exe");
    //void* addrEntityCreateFunc = (void*) (hNier + 0x4FDB50);
    void* addrEntityCreateFunc = (void*) (hNier + 0x7459B0);
    if (MH_CreateHook(addrEntityCreateFunc, (LPVOID)HookedEntityCreateFunc, (LPVOID*)&ogEntityCreateFunc) != MH_OK)
    {
        ConsoleWriteColor(FOREGROUND_RED, "Cant create hook\n");
        return;
    }

    MH_EnableHook(addrEntityCreateFunc);

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

    // If we do our DirectX hook before the game gets to the main menu we crash horribly, should just detect when this happens instead of waiting but w/e
    Sleep(5000);
    
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