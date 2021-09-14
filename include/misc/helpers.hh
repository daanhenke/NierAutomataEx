#pragma once

template <typename T>
inline T* AtOffset(void* thizz, size_t offset)
{
    return reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(thizz) + offset);
}

template <typename T, std::size_t index, typename ...Args>
constexpr T CallVirtualMethod(void* thizz, Args... args)
{
    return (*reinterpret_cast<T(__thiscall***)(void*, Args...)>(thizz))[index](thizz, args...);
}

#define PropAtOffset(type, name, offset) \
    type* Get ## name ## Ptr() \
    { \
        return AtOffset<type>(this, offset); \
    }

#define VirtualMethod(index, return_type, name, argsWrapper, argsRaw) \
    return_type name argsWrapper noexcept \
    { \
        return CallVirtualMethod<return_type, index> argsRaw; \
    }