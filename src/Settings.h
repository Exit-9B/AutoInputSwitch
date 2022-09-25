#pragma once

#include "Platform.h"

class Settings final
{
public:
	static Settings* GetSingleton();

	void LoadSettings();

	Platform PreferredPlatform;
};
