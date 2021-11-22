#include "Hooks.h"
#include "Offsets.h"

void Hooks::Install()
{
	InstallDeviceConnectHook();
	InstallInputManagerHook();
	InstallGamepadDeviceEnabledHook();

	logger::info("Installed all hooks"sv);
}

void Hooks::InstallDeviceConnectHook()
{
	REL::Relocation<std::uintptr_t> hook{ Offset::InputManager::ProcessEvent, 0x7E };
	REL::safe_fill(hook.address(), REL::NOP, 0x6);
}

void Hooks::InstallInputManagerHook()
{
	REL::Relocation<std::uintptr_t> hook{ Offset::BSInputDeviceManager::Initialize, 0x2A9 };
	REL::safe_fill(hook.address(), REL::NOP, 0x2);
}

void Hooks::InstallGamepadDeviceEnabledHook()
{
	REL::Relocation<std::uintptr_t> BSPCGamepadDeviceHandler_Vtbl{
		Offset::BSPCGamepadDeviceHandler::Vtbl
	};
	BSPCGamepadDeviceHandler_Vtbl.write_vfunc(0x7, IsGamepadDeviceEnabled);
}

bool Hooks::IsGamepadDeviceEnabled([[maybe_unused]] RE::BSPCGamepadDeviceHandler* a_device)
{
	return false;
}
