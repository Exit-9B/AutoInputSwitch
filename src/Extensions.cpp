#include "Extensions.h"
#include "Offsets.h"
#include <Windows.h>
#include <Xinput.h>

void SetGamepadRumbleEnabled(bool a_enabled)
{
	static REL::Relocation<void(bool)> func{ Offset::SetGamepadRumbleEnabled };
	func(a_enabled);

	if (!a_enabled) {
		static REL::Relocation<decltype(&XInputSetState)> xinputSetState{ Offset::XInputSetState };
		XINPUT_VIBRATION vibration{ 0, 0 };
		xinputSetState(0, std::addressof(vibration));
	}
}
