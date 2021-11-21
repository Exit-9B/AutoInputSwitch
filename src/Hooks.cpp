#include "Hooks.h"
#include "InputEventHandler.h"
#include "Offsets.h"

static std::array<std::uint8_t, 5> nop5{ 0x0F, 0x1F, 0x44, 0x00, 0x00 };
static std::array<std::uint8_t, 6> nop6{ 0x66, 0x0F, 0x1F, 0x44, 0x00, 0x00 };

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
	REL::Relocation<std::uintptr_t> hook{ Offset::InputManager::ProcessEvent, 0xBE };
	REL::safe_write<std::uint8_t>(hook.address(), nop6);
}

void Hooks::InstallInputManagerHook()
{
	REL::Relocation<std::uintptr_t> hook{ Offset::BSInputDeviceManager::Initialize, 0xF6 };
	REL::safe_write<std::uint8_t>(hook.address(), nop5);
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
	BSPCGamepadDeviceHandler_Vtbl.write_vfunc(0x8, IsGamepadDeviceEnabled);
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
