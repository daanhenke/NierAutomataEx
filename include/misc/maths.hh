#pragma once
#include <d3d11.h>

typedef float Vector2f[2];
typedef float Vector3f[3];
typedef float Vector4f[4];

typedef float Matrix4x4[4][4];

bool WorldToScreen(ID3D11Device* device, Vector3f position, Vector2f* screen);