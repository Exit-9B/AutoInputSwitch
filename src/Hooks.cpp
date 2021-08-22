#include "Hooks.h"
#include "InputEventHandler.h"

static std::array<std::uint8_t, 5> nop5{ 0x0F, 0x1F, 0x44, 0x00, 0x00 };
static std::array<std::uint8_t, 6> nop6{ 0x66, 0x0F, 0x1F, 0x44, 0x00, 0x00 };

void Hooks::Install()
{
	InstallDeviceConnectHook();
	InstallInputManagerHook();
	InstallUsingGamepadHook();
	InstallGamepadCursorHook();

	logger::info("Installed all hooks"sv);
}

void Hooks::InstallDeviceConnectHook()
{
	constexpr REL::ID offset1{ 67242 };
	REL::Relocation<std::uintptr_t> hook1{ offset1, 0xBE };
	REL::safe_write<std::uint8_t>(hook1.address(), nop6);
}

void Hooks::InstallInputManagerHook()
{
	constexpr REL::ID offset2{ 67313 };
	REL::Relocation<std::uintptr_t> hook2{ offset2, 0xF6 };
	REL::safe_write<std::uint8_t>(hook2.address(), nop5);
}

void Hooks::InstallUsingGamepadHook()
{
	auto& trampoline = SKSE::GetTrampoline();

	constexpr REL::ID offset3{ 67320 };
	REL::Relocation<std::uintptr_t> hook3{ offset3, 0xD };
	trampoline.write_call<6>(hook3.address(), IsUsingGamepad);
}

void Hooks::InstallGamepadCursorHook()
{
	auto& trampoline = SKSE::GetTrampoline();

	constexpr REL::ID offset4{ 67321 };
	REL::Relocation<std::uintptr_t> hook4{ offset4, 0xD };
	trampoline.write_call<6>(hook4.address(), IsUsingGamepad);
}

bool Hooks::IsUsingGamepad()
{
	auto inputEventHandler = InputEventHandler::GetSingleton();
	return inputEventHandler->IsUsingGamepad();
}
