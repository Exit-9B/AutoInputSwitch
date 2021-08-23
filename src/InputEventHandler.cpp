#include "InputEventHandler.h"
#include "Extensions.h"

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
	const auto inputDeviceManager = RE::BSInputDeviceManager::GetSingleton();
	if (inputDeviceManager) {
		inputDeviceManager->AddEventSink(this);
	}
}

auto InputEventHandler::ProcessEvent(
	const Event* a_event,
	[[maybe_unused]] RE::BSTEventSource<Event>* a_eventSource) -> RE::BSEventNotifyControl
{
	auto inputEvent = a_event ? *a_event : nullptr;
	if (inputEvent) {
		const auto device = inputEvent->GetDevice();
		switch (device) {
		case RE::INPUT_DEVICE::kKeyboard:
		case RE::INPUT_DEVICE::kMouse:
			if (_usingGamepad) {
				_usingGamepad = false;

				// Fix for hyper-sensitive mouse
				if (auto playerControls = RE::PlayerControls::GetSingleton()) {
					playerControls->data.lookInputVec.x = 0.0f;
					playerControls->data.lookInputVec.y = 0.0f;
				}

				SetGamepadRumbleEnabled(false);

				RefreshMenus();
			}
			break;
		case RE::INPUT_DEVICE::kGamepad:
			if (!_usingGamepad) {
				_usingGamepad = true;

				if (const auto iniPrefs = RE::INIPrefSettingCollection::GetSingleton()) {
					if (const auto setting = iniPrefs->GetSetting("bGamePadRumble:Controls")) {
						SetGamepadRumbleEnabled(setting->GetBool());
					}
				}

				RefreshMenus();
			}
			break;
		}
	}

	return RE::BSEventNotifyControl::kContinue;
}

bool InputEventHandler::IsUsingGamepad() const
{
	return _usingGamepad;
}

void InputEventHandler::RefreshMenus()
{
	const auto ui = RE::UI::GetSingleton();
	for (auto& menu : ui->menuStack) {
		if (menu) {
			menu->RefreshPlatform();
		}
	}
}
