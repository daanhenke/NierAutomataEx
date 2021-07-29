#include "misc/gui.hh"
#include "misc/maths.hh"
#include "mod.hh"

bool WorldToScreen(ID3D11Device* device, Vector3f position, Vector2f* screen)
{
    uintptr_t cameraPtr = reinterpret_cast<uintptr_t>(GetGameObject<void>("Camera"));
    Matrix4x4 *screenTransform = reinterpret_cast<Matrix4x4*>(cameraPtr + 0x50);

    float w = (*screenTransform)[0][3] * position[0] + (*screenTransform)[1][3] * position[1] + (*screenTransform)[2][3] * position[2] + (*screenTransform)[3][3];

    //if (w < .0001f) return false;

    float x = (*screenTransform)[0][0] * position[0] + (*screenTransform)[1][0] * position[1] + (*screenTransform)[2][0] * position[2] + (*screenTransform)[3][0];
    float y = (*screenTransform)[0][1] * position[0] + (*screenTransform)[1][1] * position[1] + (*screenTransform)[2][1] * position[2] + (*screenTransform)[3][1];

    float halfX = static_cast<float>(1440 * .5f);
    float halfY = static_cast<float>(900 * .5f);

    (*screen)[0] = halfX + halfX * x / w;
    (*screen)[1] = halfY + halfY * y / w;

    return true;
}