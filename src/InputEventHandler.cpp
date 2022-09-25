#include "InputEventHandler.h"
#include "Extensions.h"
#include "Offsets.h"

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

				SetGamepadRumbleEnabled(false);

				if (auto mouseMoveEvent = skyrim_cast<RE::MouseMoveEvent*>(inputEvent)) {
					ComputeMouseLookVector(
						mouseMoveEvent->mouseInputX,
						mouseMoveEvent->mouseInputY);
				}

				RefreshMenus();
			}
			break;
		case RE::INPUT_DEVICE::kGamepad:
			if (!_usingGamepad) {
				_usingGamepad = true;

				static REL::Relocation<RE::Setting*> gamepadRumble{
					Offset::INIPrefSetting::Controls::bGamepadRumble
				};
				SetGamepadRumbleEnabled(gamepadRumble->GetBool());

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
