#pragma once

#include <Windows.h>

uintptr_t FindPattern(const char* module_name, const char* signature);
unsigned int CalculateRGB(double ratio);