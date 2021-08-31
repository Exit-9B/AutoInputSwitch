#include "Hooks.h"

static std::array<std::uint8_t, 5> nop5{ 0x0F, 0x1F, 0x44, 0x00, 0x00 };
static std::array<std::uint8_t, 6> nop6{ 0x66, 0x0F, 0x1F, 0x44, 0x00, 0x00 };

void Hooks::Install()
{
	InstallDeviceConnectHook();
	InstallInputManagerHook();
	InstallGamepadDeviceEnabledHook();

	logger::info("Installed all hooks"sv);
}

void Hooks::InstallDeviceConnectHook()
{
	constexpr REL::ID InputManager_ProcessEvent_offset{ 67242 };
	REL::Relocation<std::uintptr_t> hook{ InputManager_ProcessEvent_offset, 0xBE };
	REL::safe_write<std::uint8_t>(hook.address(), nop6);
}

void Hooks::InstallInputManagerHook()
{
	constexpr REL::ID BSInputDeviceManager_Initialize_offset{ 67313 };
	REL::Relocation<std::uintptr_t> hook{ BSInputDeviceManager_Initialize_offset, 0xF6 };
	REL::safe_write<std::uint8_t>(hook.address(), nop5);
}

void Hooks::InstallGamepadDeviceEnabledHook()
{
	REL::Relocation<std::uintptr_t> BSPCGamepadDeviceHandler_Vtbl{ REL::ID{ 560029 } };
	BSPCGamepadDeviceHandler_Vtbl.write_vfunc(0x8, IsGamepadDeviceEnabled);
}

bool Hooks::IsGamepadDeviceEnabled([[maybe_unused]] RE::BSPCGamepadDeviceHandler* a_device)
{
	return false;
}
