#include "InputEventHandler.h"
#include "Extensions.h"
#include "Offsets.h"
#include "Settings.h"

InputEventHandler::InputEventHandler()
{
	const auto deviceManager = RE::BSInputDeviceManager::GetSingleton();
	const auto gamepadHandler = deviceManager ? deviceManager->GetGamepadHandler() : nullptr;
	_usingGamepad = gamepadHandler && gamepadHandler->IsEnabled();
}

auto InputEventHandler::GetSingleton() -> InputEventHandler*
{
	static InputEventHandler singleton;
	return std::addressof(singleton);
}

void InputEventHandler::Register()
{
	_preferredPlatform = Settings::GetSingleton()->PreferredPlatform;

	if (_preferredPlatform == Platform::PC) {
		_usingGamepad = false;
		SetGamepadRumbleEnabled(false);
		// We don't need to register
		return;
	}

	if (const auto inputDeviceManager = RE::BSInputDeviceManager::GetSingleton()) {
		inputDeviceManager->AddEventSink(this);
	}
}

auto InputEventHandler::ProcessEvent(
	const Event* a_event,
	[[maybe_unused]] RE::BSTEventSource<Event>* a_eventSource) -> RE::BSEventNotifyControl
{
	auto inputEvent = a_event ? *a_event : nullptr;
	while (inputEvent) {
		ProcessInput(*inputEvent);
		inputEvent = inputEvent->next;
	}

	return RE::BSEventNotifyControl::kContinue;
}

static bool IsRunningOnSteamDeck()
{
	static bool hasSteamDeckSupport = []()
	{
		// class size increased from 0x180 to 0x210
		const auto vtbl = REL::Relocation<std::uintptr_t*>(Offset::BSWin32SystemUtility::Vtbl);
		const auto dtor = vtbl.get()[0];
		return REL::make_pattern<"BA 10 02 00 00">().match(dtor + 0xA4);
	}();

	if (!hasSteamDeckSupport) {
		return false;
	}

	const auto systemUtility = RE::BSWin32SystemUtility::GetSingleton();
	return systemUtility && systemUtility->isRunningOnSteamDeck;
}

bool InputEventHandler::IsUsingGamepad() const
{
	if (IsRunningOnSteamDeck()) {
		return true;
	}

	switch (_preferredPlatform) {
	case Platform::PC:
		return false;

	case Platform::Gamepad:
		if (const auto inputDeviceManager = RE::BSInputDeviceManager::GetSingleton()) {
			return inputDeviceManager->IsGamepadEnabled();
		}

		return false;

	default:
		return _usingGamepad;
	}
}

void InputEventHandler::ProcessInput(const RE::InputEvent& a_event)
{
	if (const auto buttonEvent = a_event.AsButtonEvent()) {
		if (buttonEvent->IsRepeating()) {
			return;
		}
	}

	const auto device = a_event.GetDevice();
	switch (device) {
	case RE::INPUT_DEVICE::kKeyboard:
	case RE::INPUT_DEVICE::kMouse:
		if (IsUsingGamepad()) {
			if (_preferredPlatform != Platform::Gamepad) {
				_usingGamepad = false;

				SetGamepadRumbleEnabled(false);
			}

			// This code should run as well for gamepad users who touch the mouse
			if (const auto mouseMoveEvent = skyrim_cast<const RE::MouseMoveEvent*>(&a_event)) {
				ComputeMouseLookVector(
					mouseMoveEvent->mouseInputX,
					mouseMoveEvent->mouseInputY);
			}

			if (_preferredPlatform != Platform::Gamepad) {
				RefreshMenus();
			}
		}
		break;

	case RE::INPUT_DEVICE::kGamepad:
		if (!IsUsingGamepad()) {
			if (_preferredPlatform != Platform::PC) {
				_usingGamepad = true;

				static REL::Relocation<RE::Setting*> gamepadRumble{
					Offset::INIPrefSetting::Controls::bGamepadRumble
				};
				SetGamepadRumbleEnabled(gamepadRumble->GetBool());

				RefreshMenus();
			}
		}
		break;
	}
}

void InputEventHandler::RefreshMenus()
{
	auto task = SKSE::GetTaskInterface();
	if (task) {
		task->AddUITask(DoRefreshMenus);
	}
}

void InputEventHandler::DoRefreshMenus()
{
	const auto ui = RE::UI::GetSingleton();
	for (auto& menu : ui->menuStack) {
		if (menu) {
			menu->RefreshPlatform();
		}
	}
}

void InputEventHandler::ComputeMouseLookVector(
	std::int32_t a_mouseInputX,
	std::int32_t a_mouseInputY)
{
	auto playerControls = RE::PlayerControls::GetSingleton();

	if (!playerControls)
		return;

	RE::NiPoint2& lookVec = playerControls->data.lookInputVec;

	static REL::Relocation<RE::Setting*> fMouseHeadingSensitivity{
		Offset::INIPrefSetting::Controls::fMouseHeadingSensitivity
	};
	static REL::Relocation<RE::Setting*> fMouseHeadingXScale{
		Offset::INISetting::Controls::fMouseHeadingXScale
	};
	static REL::Relocation<RE::Setting*> fMouseHeadingYScale{
		Offset::INISetting::Controls::fMouseHeadingYScale
	};
	static REL::Relocation<float*> secondsSinceLastFrameRealTime{
		Offset::SecondsSinceLastFrameRealTime
	};

	float userSensitivity = fMouseHeadingSensitivity->GetFloat();
	float xScale = fMouseHeadingXScale->GetFloat();
	float yScale = fMouseHeadingYScale->GetFloat() / 42.5f;
	float timeDelta = *secondsSinceLastFrameRealTime.get();

	lookVec.x = ((userSensitivity * xScale) / timeDelta) * a_mouseInputX;
	lookVec.y = ((userSensitivity * yScale) / timeDelta) * -a_mouseInputY;
}
