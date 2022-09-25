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
			// SkyrimSE 1.6.318.0: 0x1E85C38
			inline constexpr REL::ID fMouseHeadingXScale(381989);
			// SkyrimSE 1.6.318.0: 0x1E85C50
			inline constexpr REL::ID fMouseHeadingYScale(381992);
		}
	}

	namespace INIPrefSetting
	{
		namespace Controls
		{
			// SkyrimSE 1.6.318.0: 0x1E85C20
			inline constexpr REL::ID fMouseHeadingSensitivity(381986);
			// SkyrimSE 1.6.318.0: 0x1E85B60
			inline constexpr REL::ID bGamepadRumble(381962);
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
