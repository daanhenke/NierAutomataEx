#pragma once

#include "misc/helpers.hh"
#include "misc/maths.hh"

typedef unsigned long long EntityHandle;

typedef float Vector4[4];
typedef Vector4 Vector3Aligned;

typedef struct ModelInfo
{
    Vector4 tint;
} ModelInfo;

class EntityInfo;

class Entity
{
public:
    PropAtOffset(uintptr_t, VTable, 0x0)
    PropAtOffset(Vector3Aligned , Position, 0x50)
    PropAtOffset(int, ObjectId, 0x5B8)
    PropAtOffset(ModelInfo*, ModelInfo, 0x540)
    PropAtOffset(EntityInfo*, EntityInfo, 0x610)

    VirtualMethod(29, bool, IsAliveAndHasEntityInfo, (), (this))
    VirtualMethod(86, int, GetLevel, (), (this))
    VirtualMethod(104, void, SetLevel, (int level), (this, level))

    bool IsEnemy()
    {
        // 00007FF6A40CFE60
        return *GetVTablePtr() == 0x00007FF6A40AB888;
    }
};


typedef struct EntityInfo
{
    void* somepointer;
    char entity_name[32];
    unsigned int object_id;
    unsigned char flags;
    char pad0[3];
    EntityHandle handle;
    char pad1[4];
    char* data[1];
    Entity* entity;
} EntityInfo;

typedef struct EntityInfoListEntry
{
    EntityHandle handle;
    EntityInfo* info;
} EntityInfoListEntry;

typedef struct EntityInfoList
{
    unsigned int item_count;
    unsigned int size;
    unsigned int base;
    unsigned int shift;
    EntityInfoListEntry* items;
} EntityInfoList;