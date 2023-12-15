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

	logger::info("Finished installing hooks"sv);
}

void Hooks::InstallDeviceConnectHook()
{
	auto hook = REL::Relocation<std::uintptr_t>(Offset::ControlMap::MapToUserEvent, 0x7E);
	if (!REL::make_pattern<"88 87">().match(hook.address())) {
		logger::critical("Failed to install MapToUserEvent hook"sv);
		return;
	}

	REL::safe_fill(hook.address(), REL::NOP, 0x6);

	const auto code = std::vector<std::uint8_t>{ 0xB0, 0x00 };
	REL::safe_write(hook.address(), std::span(code));
}

void Hooks::InstallInputManagerHook()
{
	static constinit auto pattern = REL::make_pattern<"B1 01 EB 02">();
	auto hook = REL::Relocation<std::uintptr_t>(Offset::BSInputDeviceManager::Ctor, 0x2A9);

	if (!pattern.match(hook.address())) {
		hook = REL::Relocation<std::uintptr_t>(Offset::BSInputDeviceManager::Ctor, 0x2B7);

		if (!pattern.match(hook.address())) {
			logger::critical("Failed to install BSInputDeviceManager hook"sv);
			return;
		}
	}

	REL::safe_fill(hook.address(), REL::NOP, 0x4);
}

void Hooks::InstallUsingGamepadHook()
{
	auto& trampoline = SKSE::GetTrampoline();

	static constinit auto pattern = REL::make_pattern<"48 8B 01 FF 50 38">();
	// this ID is gone in 1.6.1130
	// TODO: revisit this logic when GOG update drops
	auto hook = REL::Relocation<std::uintptr_t>(
		Offset::BSInputDeviceManager::QUsingGamepad_OLD,
		0xD);

	if (!pattern.match(hook.address())) {
		hook = REL::Relocation<std::uintptr_t>(Offset::BSInputDeviceManager::QUsingGamepad, 0x20);

		if (!pattern.match(hook.address())) {
			logger::critical("Failed to install qUsingGamepad hook"sv);
			return;
		}
	}

	trampoline.write_call<6>(hook.address(), IsUsingGamepad);
}

void Hooks::InstallGamepadCursorHook()
{
	auto& trampoline = SKSE::GetTrampoline();

	auto hook = REL::Relocation<std::uintptr_t>(
		Offset::BSInputDeviceManager::IsGamepadConnected,
		0xD);

	if (!REL::make_pattern<"48 8B 01 FF 50 38">().match(hook.address())) {
		logger::critical("Failed to install IsGamepadConnected hook"sv);
		return;
	}

	trampoline.write_call<6>(hook.address(), IsUsingGamepad);
}

void Hooks::InstallGamepadDeviceEnabledHook()
{
	auto BSPCGamepadDeviceHandler_Vtbl = REL::Relocation<std::uintptr_t>(
		Offset::BSPCGamepadDeviceHandler::Vtbl);
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
