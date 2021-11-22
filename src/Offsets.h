#pragma once

namespace Offset
{
	namespace BSInputDeviceManager
	{
		// SkyrimSE 1.6.318.0: 0xC3AF10
		inline constexpr REL::ID Initialize(68615);
		// SkyrimSE 1.6.318.0: 0xC3B4F0
		inline constexpr REL::ID IsUsingGamepad(68622);
		// SkyrimSE 1.6.318.0: 0xC3B530
		inline constexpr REL::ID GamepadControlsCursor(68624);
	}

	namespace BSPCGamepadDeviceHandler
	{
		// SkyrimSE 1.6.318.0: 0x184EC10
		inline constexpr REL::ID Vtbl(236813);
	}

	namespace INISetting
	{
		namespace Controls
		{
			// SkyrimSE 1.6.318.0: 0x1E85C40
			inline constexpr REL::ID fMouseHeadingXScale_Value(381990);
			// SkyrimSE 1.6.318.0: 0x1E85C58
			inline constexpr REL::ID fMouseHeadingYScale_Value(381993);
		}
	}

	namespace INIPrefSetting
	{
		namespace Controls
		{
			// SkyrimSE 1.6.318.0: 0x1E85C28
			inline constexpr REL::ID fMouseHeadingSensitivity_Value(381987);
			// SkyrimSE 1.6.318.0: 0x1E85B68
			inline constexpr REL::ID bGamepadRumble_Value(381963);
		}
	}

	namespace InputManager
	{
		// SkyrimSE 1.6.318.0: 0xC375C0
		inline constexpr REL::ID ProcessEvent(68542);
	}

	// SkyrimSE 1.6.318.0: 0x30064CC
	inline constexpr REL::ID SecondsSinceLastFrameRealTime(410200);

	// SkyrimSE 1.6.318.0: 0xC36910
	inline constexpr REL::ID SetGamepadRumbleEnabled(68531);

	// SkyrimSE 1.6.318.0: 0xC3E323
	inline constexpr REL::ID XInputSetState(68735);
}
