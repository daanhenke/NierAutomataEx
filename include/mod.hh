#pragma once

void LoadMod();
void UnloadMod();

#include <map>
#include <string>
#include <Windows.h>

#include "config.hh"
#include "automata/entities.hh"

extern std::map<std::string, uintptr_t> gGameOffsets;
inline uintptr_t GetGameOffset(std::string name)
{
    if (gGameOffsets.find(name) != gGameOffsets.end()) return gGameOffsets[name];

    return 0;
}

template <typename T>
inline T *GetGameObject(std::string name)
{
    static uintptr_t nierBase = reinterpret_cast<uintptr_t>(GetModuleHandleA("NierAutomata.exe"));

    uintptr_t offset = GetGameOffset(name);
    if (offset == 0) return nullptr;

    return reinterpret_cast<T*>(nierBase + offset);
}

extern EntityInfoList* gEntityList;
extern Entity** gPlayerEntityPtr;

Entity* GetPlayerEntity();

void ApplyEntityMods();