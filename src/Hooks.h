#pragma once

namespace Hooks
{
	void Install();

	// Prevent ignoring mouse/keyboard when connecting a gamepad
	void InstallDeviceConnectHook();

	// Prevent ignoring mouse/keyboard on startup
	void InstallInputManagerHook();

	// Dynamic button art, cursor show/hide, and look sensitivity
	void InstallUsingGamepadHook();

	// Dynamic map menu cursor sensitivity
	void InstallGamepadCursorHook();

	// Trick SKSE StartRemapMode (e.g. for SkyUI MCM) into allowing all inputs
	void InstallGamepadDeviceEnabledHook();

	bool IsUsingGamepad();
	bool IsGamepadDeviceEnabled(RE::BSPCGamepadDeviceHandler* a_device);
}
