#include "Settings.h"
#include <SimpleIni.h>

Settings* Settings::GetSingleton()
{
	static Settings singleton{};
	return &singleton;
}

void Settings::LoadSettings()
{
	CSimpleIniA ini{};
	ini.SetUnicode();
	ini.LoadFile(fmt::format(R"(.\Data\SKSE\Plugins\{}.ini)", Plugin::NAME).c_str());

	PreferredPlatform = static_cast<Platform>(
		ini.GetLongValue("Input", "iPreferredPlatform", static_cast<long>(Platform::Auto)));
}
