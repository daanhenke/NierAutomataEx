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
std::vector<unsigned int> gUniqueEntityIds;

void LogEntities()
{
    uintptr_t nier_handle = reinterpret_cast<uintptr_t>(GetModuleHandleA(nullptr));
    EntityInfoList* list = reinterpret_cast<EntityInfoList*>((nier_handle + OffEntityInfoList));

    Log("List ptr: %llx\n", list);

    for (unsigned int i = 0; i < list->item_count; i++)
    {
        auto entry = list->items[i];

        if (entry.info == nullptr) continue;

        if (std::find(gUniqueEntityIds.begin(), gUniqueEntityIds.end(), entry.info->object_id) == gUniqueEntityIds.end())
        {
            gUniqueEntityIds.push_back(entry.info->object_id);
            Log("Found new object ID: %x, name: '%s', ptr: %llx\n", entry.info->object_id, entry.info->entity_name, entry.info->entity);
        }
        
        if (entry.info->entity == nullptr) continue;

        if (true)
        {
            uintptr_t entity = (uintptr_t) entry.info->entity;

            auto modelInfo = *(ModelInfo**) (entity + OffEntityModelInfo);
            auto hp = (int*)(entity + OffEntityHP);

            Entity* ent = (Entity*) entity;

            if (modelInfo == nullptr) continue;

            auto name = std::string(entry.info->entity_name);
            if (entry.info->object_id >= 0x20000 && entry.info->object_id < 0x30000)
            {
                if (ent->GetLevel() < 69 && ent->GetLevel() != 63)
                {
                    Log("Boosted entity %llx to level 69\n", ent);
                    ent->SetLevel(99);
                }
                modelInfo->tint[0] = 64.f;
            }
            else if (name == "Player")
            {
                modelInfo->tint[1] = 64.f;
                ent->SetLevel(99);
            }
            else
            {
                modelInfo->tint[2] = 1.f;
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

    while (true)
    {
        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
        }

        if (GetAsyncKeyState(VK_END) & 1)
        {
            break;
        }

        LogEntities();
        Sleep(10);
    }

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