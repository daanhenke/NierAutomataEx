#include "features/debuggui.hh"
#include "imgui.h"
#include "mod.hh"
#include "misc/console.hh"
#include "misc/maths.hh"
#include "misc/gui.hh"

typedef float Vector3Aligned[4];

void DebugGuiRender()
{
    uintptr_t localPlayer = *reinterpret_cast<uintptr_t*>(GetGameObject<void>("LocalPlayerPtr"));
    if (localPlayer == 0) return;
    int *localPlayerObjectId = reinterpret_cast<int*>(localPlayer + 0x5B8);
    Vector3Aligned *localPlayerPosition = reinterpret_cast<Vector3Aligned*>(localPlayer + 0x50);

    Vector2f screenPos = {0.f, 0.f};
    //bool isOnScreen = WorldToScreen(d3dDevice, *localPlayerPosition, &screenPos);
    //if (! isOnScreen) return;
    //ConsoleWrite("On screen: %f, %f\n", screenPos[0], screenPos[1]);

    ImGui::SetNextWindowPos(ImVec2(screenPos[0], screenPos[1]), ImGuiCond_Once);

    bool isOpen = true;
    ImGui::Begin("Local Player");
    ImGui::InputInt("Object ID", localPlayerObjectId, 1, 100, ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputFloat3("Position", reinterpret_cast<float*>(localPlayerPosition));
    ImGui::End();
}