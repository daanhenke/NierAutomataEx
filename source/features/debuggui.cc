#include "features/debuggui.hh"
#include "imgui.h"
#include "mod.hh"
#include "misc/console.hh"
#include "misc/maths.hh"
#include "misc/gui.hh"
#include "automata/entities.hh"


void DebugGuiRender()
{
    Entity* localPlayer = *GetGameObject<Entity*>("LocalPlayerPtr");
    if (localPlayer == nullptr) return;
    int *localPlayerObjectId = localPlayer->GetObjectIdPtr();
    Vector3Aligned *localPlayerPosition = localPlayer->GetPositionPtr();

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