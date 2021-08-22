#pragma once

namespace Hooks
{
	void Install();

	void InstallDeviceConnectHook();
	void InstallInputManagerHook();
	void InstallUsingGamepadHook();
	void InstallGamepadCursorHook();

	bool IsUsingGamepad();
}
