#pragma once

class InputEventHandler : public RE::BSInputDeviceManager::Sink
{
public:
	using Event = RE::InputEvent*;

	static auto GetSingleton() -> InputEventHandler*;
	void Register();

	auto ProcessEvent(const Event* a_event, RE::BSTEventSource<Event>* a_eventSource)
		-> RE::BSEventNotifyControl override;

	bool IsUsingGamepad() const;

private:
	void RefreshMenus();

	bool _usingGamepad;
};
