#pragma once

const int OffLocalPlayer = 0x01020918;
const int OffEntityInfoList = 0x1029510;
const int OffEntityHP = 0x00858;
const int OffEntityMaxHP = 0x0085C;
const int OffEntityMappedModel = 0x00398;
const int OffEntityModelInfo = 0x00540;

typedef unsigned long long EntityHandle;

typedef float Vector4[4];

typedef struct ModelInfo
{
    Vector4 tint;
} ModelInfo;

class Entity
{
public:
    virtual void f0() = 0;
    virtual void f1() = 0;
    virtual void f2() = 0;
    virtual void f3() = 0;
    virtual void f4() = 0;
    virtual void f5() = 0;
    virtual void f6() = 0;
    virtual void f7() = 0;
    virtual void f8() = 0;
    virtual void f9() = 0;
    
    virtual void f10() = 0;
    virtual void f11() = 0;
    virtual void f12() = 0;
    virtual void f13() = 0;
    virtual void f14() = 0;
    virtual void f15() = 0;
    virtual void f16() = 0;
    virtual void f17() = 0;
    virtual void f18() = 0;
    virtual void f19() = 0;
    
    virtual void f20() = 0;
    virtual void f21() = 0;
    virtual void f22() = 0;
    virtual void f23() = 0;
    virtual void f24() = 0;
    virtual void f25() = 0;
    virtual void f26() = 0;
    virtual void f27() = 0;
    virtual void f28() = 0;
    virtual void f29() = 0;
    
    virtual void f30() = 0;
    virtual void f31() = 0;
    virtual void f32() = 0;
    virtual void f33() = 0;
    virtual void f34() = 0;
    virtual void f35() = 0;
    virtual void f36() = 0;
    virtual void f37() = 0;
    virtual void f38() = 0;
    virtual void f39() = 0;
    
    virtual void f40() = 0;
    virtual void f41() = 0;
    virtual void f42() = 0;
    virtual void f43() = 0;
    virtual void f44() = 0;
    virtual void f45() = 0;
    virtual void f46() = 0;
    virtual void f47() = 0;
    virtual void f48() = 0;
    virtual void f49() = 0;
    
    virtual void f50() = 0;
    virtual void f51() = 0;
    virtual void f52() = 0;
    virtual void f53() = 0;
    virtual void f54() = 0;
    virtual void f55() = 0;
    virtual void f56() = 0;
    virtual void f57() = 0;
    virtual void f58() = 0;
    virtual void f59() = 0;
    
    virtual void f60() = 0;
    virtual void f61() = 0;
    virtual void f62() = 0;
    virtual void f63() = 0;
    virtual void f64() = 0;
    virtual void f65() = 0;
    virtual void f66() = 0;
    virtual void f67() = 0;
    virtual void f68() = 0;
    virtual void f69() = 0;
    
    virtual void f70() = 0;
    virtual void f71() = 0;
    virtual void f72() = 0;
    virtual void f73() = 0;
    virtual void f74() = 0;
    virtual void f75() = 0;
    virtual void f76() = 0;
    virtual void f77() = 0;
    virtual void f78() = 0;
    virtual void f79() = 0;
    
    virtual void f80() = 0;
    virtual void f81() = 0;
    virtual void f82() = 0;
    virtual void f83() = 0;
    virtual void f84() = 0;
    virtual void f85() = 0;
    virtual int GetLevel() = 0;
    virtual void f87() = 0;
    virtual void f88() = 0;
    virtual void f89() = 0;
    
    virtual void f90() = 0;
    virtual void f91() = 0;
    virtual void f92() = 0;
    virtual void f93() = 0;
    virtual void f94() = 0;
    virtual void f95() = 0;
    virtual void f96() = 0;
    virtual void f97() = 0;
    virtual void f98() = 0;
    virtual void f99() = 0;
    
    virtual void f100() = 0;
    virtual void f101() = 0;
    virtual void f102() = 0;
    virtual void f103() = 0;
    virtual void SetLevel(int level) = 0;
    virtual void f105() = 0;
    virtual void f106() = 0;
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
