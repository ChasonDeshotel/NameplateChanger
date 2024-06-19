#pragma once

#include "GuiBase.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include <memory>
#include <string>

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

class NameplateChanger: public BakkesMod::Plugin::BakkesModPlugin
	,public SettingsWindowBase // Uncomment if you wanna render your own tab in the settings menu
	//,public PluginWindowBase // Uncomment if you want to render your own plugin window
{
public:
	virtual void onLoad();
	//void onUnload() override; // Uncomment and implement if you need a unload method

public:
	void RenderSettings() override; // Uncomment if you wanna render your own tab in the settings menu
	//void RenderWindow() override; // Uncomment if you want to render your own plugin window

private:
    std::shared_ptr<bool> enabled;

    struct FString {
        wchar_t* Data;
        int32_t Count;
        int32_t Max;

        FString(const std::string& str) {
            Count = static_cast<int32_t>(str.length());
            Max = Count + 1;
            Data = new wchar_t[Max];
            mbstowcs(Data, str.c_str(), Count);
            Data[Count] = L'\0';
        }

        ~FString() {
            delete[] Data;
        }
    };

    void HookSetPlayerName();
};

extern std::shared_ptr<CVarManagerWrapper> _globalCvarManager;
