#include "Extensions.h"
#include <Windows.h>
#include <Xinput.h>

void SetGamepadRumbleEnabled(bool a_enabled)
{
	static REL::Relocation<void(bool)> func{ REL::ID(67222) };
	func(a_enabled);

	if (!a_enabled) {
		static REL::Relocation<decltype(&XInputSetState)> xinputSetState{ REL::ID(67429) };
		XINPUT_VIBRATION vibration{ 0, 0 };
		xinputSetState(0, std::addressof(vibration));
	}
}
