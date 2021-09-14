#include "features/debuggui.hh"
#include "imgui.h"
#include "mod.hh"
#include "misc/console.hh"
#include "misc/maths.hh"
#include "misc/gui.hh"
#include "automata/entities.hh"
#include <string>

std::map<std::string, player_choice_t> choosablePlayers =
{
    { "2B", Pl2B},
    { "9S", Pl9S},
    { "A2", PlA2}
};

std::string currentChosenPlayer = "2B";

void DebugGuiRender()
{
    if (ImGui::Begin("AutomataEx"))
    {
        if (ImGui::BeginTabBar("Sections", ImGuiTabBarFlags_None))
        {
            if (ImGui::BeginTabItem("Difficulty & Balance"))
            {
                ImGui::Checkbox("Enable level scaling", &gConfig.EnableLevelScaling);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Hacks / Misc"))
            {
                ImGui::Checkbox("Enable player character forcing", &gConfig.EnablePlayerForcing);
                if (ImGui::BeginCombo("Character", currentChosenPlayer.c_str()))
                {
                    for (auto item : choosablePlayers)
                    {
                        bool isCurrent = item.first == currentChosenPlayer;
                        if (ImGui::Selectable(item.first.c_str(), isCurrent))
                        {
                            currentChosenPlayer = item.first;
                            gConfig.PlayerForcingValue = item.second;
                        }

                        if (isCurrent)
                        {
                            ImGui::SetItemDefaultFocus();
                        }
                    }

                    ImGui::EndCombo();
                }

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Debug"))
            {
                GuiLogPtr("PlayerPtr", GetPlayerEntity());
                ImGui::Checkbox("Show entity overlay", &gConfig.EnableEntityOverlay);
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End();
    }

    for (int i = 0; i < gEntityList->item_count; i++)
    {
        auto entry = gEntityList->items[i];
        auto info = entry.info;

        if (info == nullptr) continue;
        auto entity = info->entity;
        if (entity == nullptr) continue;

        auto object_id = *entity->GetObjectIdPtr();
        auto entinfo = *entity->GetEntityInfoPtr();
        if (gConfig.EnableEntityOverlay)
        {
            Vector2f winCoords;
            bool renderPlayerWnd = WorldToScreen(*entity->GetPositionPtr(), &winCoords);
            std::string winName = "ent" + std::to_string(reinterpret_cast<uintptr_t>(entity));
            ImGui::SetNextWindowPos(ImVec2(winCoords[0], winCoords[1]));
            ImGui::SetNextWindowSize(ImVec2(150, 60));
            if (ImGui::Begin(winName.c_str(), nullptr, 1))
            {
                ImGui::InputScalar("ID", ImGuiDataType_U32, &object_id, 0, 0, "%x", ImGuiInputTextFlags_CharsHexadecimal);
                ImGui::InputText("Name", entinfo->entity_name, 32);
                ImGui::End();
            }
        }
    }
}