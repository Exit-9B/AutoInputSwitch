#pragma once

namespace Hooks
{
	void Install();

	// Prevent ignoring mouse/keyboard when connecting a gamepad
	void InstallDeviceConnectHook();

	// Prevent ignoring mouse/keyboard on startup
	void InstallInputManagerHook();

	// Ignore gamepad for everything except receiving inputs
	void InstallGamepadDeviceEnabledHook();

	bool IsGamepadDeviceEnabled(RE::BSPCGamepadDeviceHandler* a_device);
}
