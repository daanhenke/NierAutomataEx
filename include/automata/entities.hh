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
    PropAtOffset(int, Health, 0x858)
    PropAtOffset(int, MaxHealth, 0x85C)

    VirtualMethod(29, bool, IsAliveAndHasEntityInfo, (), (this))
    VirtualMethod(86, int, GetLevel, (), (this))
    VirtualMethod(104, void, SetLevel, (int level), (this, level))

    bool IsEnemy()
    {
        auto object_id = *GetObjectIdPtr();

        if (object_id >= 0x20000 && object_id <= 0x30000)
        {
            switch (object_id)
            {
            // Birds
            case 0x2a021:
                return false;

            default:
                return true;
            }
        }

        return false;
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