#include <utilities.hh>

#include <thread>
#include <cstdio>

// For the developer console
#define DEBUG

// Logging macro
#ifdef DEBUG
#define Log(...) printf(__VA_ARGS__)
#else
#define Log(...)
#endif 

// MSVC specific symbol that points to the start of the dll file, which also happens to be the module handle
extern "C" IMAGE_DOS_HEADER __ImageBase;
HMODULE gModuleHandle = reinterpret_cast<HMODULE>(&__ImageBase);

// For overlays and such
HWND gGameWindowHandle = nullptr;

// Always spawn a thread for exit so it can cleanup our patches without the game jumping back to them after they are gone
void DoExit()
{
    FreeLibraryAndExitThread(gModuleHandle, 0);
}

bool gIsShuttingDown = false;
void Exit()
{
    if (gIsShuttingDown) return;
    gIsShuttingDown = true;
    
    Sleep(600);

    #ifdef DEBUG
    FreeConsole();
    #endif

    CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(DoExit), nullptr, 0, nullptr);
}

#include "offsets.hh"
#include <vector>
#include <string>
#include <algorithm>

#include "minhook.h"

EntityInfoList* list = nullptr;
Entity* player_entity = nullptr;

typedef uint64_t(__fastcall* HashEntity_t)(char* entityName, uint64_t entityNameLength);
HashEntity_t origHashEntity;

//uint64_t __fastcall HashEntityHooked(char* entityName, uint64_t entityNameLength)
//{
//    static int cntr = 0;
//    if (strcmp(entityName, "PL/9S") == 0)
//    {
//        Log("Overwriting 9S with A2?\n");
//        if (cntr % 2 == 1)
//        {
//            entityName = "PL/A2";
//            Log("Overwriting 9S with A2!!!\n");
//        }
//        cntr++;
//    }
//    //Log("Looking up entity: %s\n", entityName);
//
//    return origHashEntity(entityName, entityNameLength);
//}

void LogEntities()
{
    if (MH_Initialize() != MH_OK)
    {
        Log("Couldn't initialize minhook\n");
        return;
    }

    uintptr_t nier_handle = reinterpret_cast<uintptr_t>(GetModuleHandleA(nullptr));

    Log("Located EntityInfoList: %llx\n", list);
    Log("Correct EntityInfoList: %llx\n", nier_handle + OffEntityInfoList);

    void* hashEntity = (char*) nier_handle + 0x28EBC0;
    int* pCurrentPlayerId = (int*) ((char*)nier_handle + 0x11e1c04);

    /*if (MH_CreateHook(hashEntity, &HashEntityHooked, reinterpret_cast<LPVOID*>(&origHashEntity)) != MH_OK)
    {
        Log("Failed to create hook\n");
        return;
    }

    MH_EnableHook(hashEntity);*/

    while (true)
    {
        Sleep(100);
        *pCurrentPlayerId = 65792;
        for (unsigned int i = 0; i < list->item_count; i++)
        {
            auto entry = list->items[i];

            if (entry.info == nullptr) continue;
            if (entry.info->entity == nullptr) continue;

            uintptr_t entity = (uintptr_t)entry.info->entity;
            
            auto modelInfoPtr = (ModelInfo**)(entity + OffEntityModelInfo);
            if (modelInfoPtr == nullptr) continue;
            auto modelInfo = *modelInfoPtr;

            Entity* ent = (Entity*)entity;

            if (modelInfo == nullptr) continue;

            auto name = std::string(entry.info->entity_name);
            //Log("Found an entity @ %llx, level: %x, also GetLevel @ %llx\n", ent, 0, &Entity::GetLevel);
            if (entry.info->object_id >= 0x20000 && entry.info->object_id < 0x30000 && player_entity != nullptr)
            {
                if (ent->GetLevel() < player_entity->GetLevel())
                {
                    Log("Boosted entity %llx\n", ent);
                    ent->SetLevel(player_entity->GetLevel());
                }
            }
            else if (name == "Player")
            {
                if (player_entity == nullptr)
                {
                    Log("Found player: %llx\n", ent);
                }
                player_entity = ent;
            }
        }
    }
    
}

void Initialize()
{
    // Get a handle to the game's window
    gGameWindowHandle = FindWindowA(nullptr, "NieR:Automata");

    #ifdef DEBUG
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stderr);
    freopen("CONOUT$", "w", stdout);
    #endif

    Log("Window handle: %x\n", gGameWindowHandle);

    uintptr_t list_accessor = FindPattern("NierAutomata.exe", "74 35 8b c2 c1 f8 08 0f b7 c8 3b 0d") + 12;
    unsigned int reloffset = *reinterpret_cast<unsigned int*>(list_accessor);
    uintptr_t list_real_addr = (uintptr_t)(list_accessor + reloffset + 4);

    Log("Found EntityInfoList the hopefully? crossplatform way: %llx\n", list_real_addr);
    list = reinterpret_cast<EntityInfoList*>(list_real_addr);

    LogEntities();

    Log("Exiting...\n");
    
    Exit();
}

// Cross platform alternative for DllMain
class MainT
{
public:
    MainT()
    {
    CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Initialize), nullptr, 0, nullptr);
    }

    ~MainT()
    {
        //Exit();
    }
};

MainT _main;