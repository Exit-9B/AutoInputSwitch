#include "Hooks.h"
#include "InputEventHandler.h"
#include "Offsets.h"

void Hooks::Install()
{
	InstallDeviceConnectHook();
	InstallInputManagerHook();
	InstallUsingGamepadHook();
	InstallGamepadCursorHook();
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

void Hooks::InstallUsingGamepadHook()
{
	auto& trampoline = SKSE::GetTrampoline();

	REL::Relocation<std::uintptr_t> hook{ Offset::BSInputDeviceManager::IsUsingGamepad, 0xD };
	trampoline.write_call<6>(hook.address(), IsUsingGamepad);
}

void Hooks::InstallGamepadCursorHook()
{
	auto& trampoline = SKSE::GetTrampoline();

	REL::Relocation<std::uintptr_t> hook{
		Offset::BSInputDeviceManager::GamepadControlsCursor, 0xD
	};
	trampoline.write_call<6>(hook.address(), IsUsingGamepad);
}

void Hooks::InstallGamepadDeviceEnabledHook()
{
	REL::Relocation<std::uintptr_t> BSPCGamepadDeviceHandler_Vtbl{
		Offset::BSPCGamepadDeviceHandler::Vtbl
	};
	BSPCGamepadDeviceHandler_Vtbl.write_vfunc(0x7, IsGamepadDeviceEnabled);
}

bool Hooks::IsUsingGamepad()
{
	auto inputEventHandler = InputEventHandler::GetSingleton();
	return inputEventHandler && inputEventHandler->IsUsingGamepad();
}

bool Hooks::IsGamepadDeviceEnabled(RE::BSPCGamepadDeviceHandler* a_device)
{
	bool isEnabled = a_device->currentPCGamePadDelegate != nullptr;

	if (isEnabled) {
		auto playerControls = RE::PlayerControls::GetSingleton();
		bool playerRemapMode = playerControls && playerControls->data.remapMode;

		auto menuControls = RE::MenuControls::GetSingleton();
		bool menuRemapMode = menuControls && menuControls->remapMode;

		if (playerRemapMode || menuRemapMode) {
			return IsUsingGamepad();
		}
	}

	return isEnabled;
}
