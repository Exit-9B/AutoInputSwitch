#pragma once

namespace Offset
{
	namespace BSInputDeviceManager
	{
		// SkyrimSE 1.5.97.0: 0xC14EC0
		inline constexpr REL::ID Initialize(67313);
		// SkyrimSE 1.5.97.0: 0xC15240
		inline constexpr REL::ID IsUsingGamepad(67320);
		// SkyrimSE 1.5.97.0: 0xC15280
		inline constexpr REL::ID GamepadControlsCursor(67321);
	}

	namespace BSPCGamepadDeviceHandler
	{
		// SkyrimSE 1.5.97.0: 0x175E848
		inline constexpr REL::ID Vtbl(560029);
	}

	namespace INISetting
	{
		namespace Controls
		{
			// SkyrimSE 1.5.97.0: 0x1DF1830
			inline constexpr REL::ID fMouseHeadingXScale_Value(509519);
			// SkyrimSE 1.5.97.0: 0x1DF1848
			inline constexpr REL::ID fMouseHeadingYScale_Value(509521);
		}
	}

	namespace INIPrefSetting
	{
		namespace Controls
		{
			// SkyrimSE 1.5.97.0: 0x1DF1818
			inline constexpr REL::ID fMouseHeadingSensitivity_Value(509517);
			// SkyrimSE 1.5.97.0: 0x1DF1758
			inline constexpr REL::ID bGamepadRumble_Value(509501);
		}
	}

	namespace InputManager
	{
		// SkyrimSE 1.5.97.0: 0xC11600
		inline constexpr REL::ID ProcessEvent(67242);
	}

	// SkyrimSE 1.5.97.0: 0x2F6B94C
	inline constexpr REL::ID SecondsSinceLastFrameRealTime(523661);

	// SkyrimSE 1.5.97.0: 0xC10840
	inline constexpr REL::ID SetGamepadRumbleEnabled(67222);

	// SkyrimSE 1.5.97.0: 0xC17FA4
	inline constexpr REL::ID XInputSetState(67429);
}
