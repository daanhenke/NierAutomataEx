#pragma once

#include <Windows.h>

extern "C" IMAGE_DOS_HEADER __ImageBase;
static HMODULE gModuleHandle = reinterpret_cast<HMODULE>(&__ImageBase);