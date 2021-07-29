#pragma once

#include <cstdio>
#include <Windows.h>

void ConsoleInitialize();
void ConsoleDestroy();

void ConsoleColor(WORD color = 0);

#define ConsoleWriteRaw(...) printf(__VA_ARGS__)
#define ConsoleWriteColor(color, ...) ConsoleColor(color); ConsoleWriteRaw(__VA_ARGS__)
#define ConsoleWrite(...) ConsoleWriteColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, __VA_ARGS__)
#define ConsoleWriteError(...) ConsoleWriteColor(FOREGROUND_RED, __VA_ARGS__)