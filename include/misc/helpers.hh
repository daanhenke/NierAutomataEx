#pragma once

template <typename T>
inline T* AtOffset(void* thizz, size_t offset)
{
    return reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(thizz) + offset);
}

#define PropAtOffset(type, name, offset) \
    type* Get ## name ## Ptr() \
    { \
        return AtOffset<type>(this, offset); \
    }