#include "pch.h"
#include "NameplateChanger.h"
#include <string>

BAKKESMOD_PLUGIN(NameplateChanger, "Nameplate Changer", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void NameplateChanger::onLoad()
{
    _globalCvarManager = cvarManager;
    // !! Enable debug logging by setting DEBUG_LOG = true in logging.h !!
    DEBUGLOG("$projectname$ debug mode enabled");

    enabled = std::make_shared<bool>(false);
    cvarManager->registerCvar("enabled", "0", "Enable the plugin", true, true, 0, true, 1).bindTo(enabled);
    cvarManager->registerCvar("custom_player_name", "DefaultName", "Sets a custom player name", true, true, 0.0f, true, 100.0f, true);

    HookSetPlayerName();

    cvarManager->getCvar("custom_player_name").addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
        DEBUGLOG("custom_player_name changed from {} to {}", oldValue, cvar.getStringValue());
	});
}

void NameplateChanger::HookSetPlayerName()
{
    gameWrapper->HookEventWithCaller<ActorWrapper>("Function TAGame.NameplateComponent_TA.SetPlayerName", [this](ActorWrapper caller, void* params, std::string eventName) {
        DEBUGLOG("SetPlayerName hook triggered");

        if (*enabled) {

            struct SetPlayerNameParams {
                char pad[0x8];
                FString playerName;
            };

            SetPlayerNameParams* setPlayerNameParams = static_cast<SetPlayerNameParams*>(params);

            std::string customName = cvarManager->getCvar("cusntom_player_name").getStringValue();

            DEBUGLOG("Setting player name to: {}", customName);

            FString newPlayerName(customName);
            setPlayerNameParams->playerName = newPlayerName;

            DEBUGLOG("Player name set successfully");
        }

        });
}

void NameplateChanger::RenderSettings()
{
    // Add input for custom player name
    static char playerName[128] = "";
    if (ImGui::InputText("Custom Player Name", playerName, IM_ARRAYSIZE(playerName))) {
        cvarManager->getCvar("custom_player_name").setValue(std::string(playerName));
    }

    bool enabledValue = *enabled;
    if (ImGui::Checkbox("Enabled", &enabledValue)) {
        cvarManager->getCvar("enabled").setValue(enabledValue);
        DEBUGLOG("Enabled changed to {}", enabledValue);
    }
}