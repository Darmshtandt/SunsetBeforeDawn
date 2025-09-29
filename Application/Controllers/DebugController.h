#pragma once

#include <Nt/Graphics/System/Window.h>
#include <Helpers/DebugSpectator.h>
#include <Controllers/MovableController.h>

class DebugController : public IController {
public:
	static constexpr const char* COMMAND_TOGGLE_FOCUS = "ToggleFocus";
	static constexpr const char* COMMAND_INCREACE_SPEED = "IncreaseSpeed";
	static constexpr const char* COMMAND_DECREACE_SPEED = "DecreaseSpeed";

public:
	DebugController(NotNull<DebugSpectator*> pDebugCamera) :
		m_pDebugSpectator(pDebugCamera),
		m_MovableController(pDebugCamera.Get())
	{
		m_Input.BindKeyCommand(Nt::Key::_ADD, COMMAND_TOGGLE_FOCUS);
		m_Input.BindMouseCommand(Nt::Mouse::_LEFT, COMMAND_INCREACE_SPEED);
		m_Input.BindMouseCommand(Nt::Mouse::_RIGHT, COMMAND_DECREACE_SPEED);
		m_Input.SetCommandActions(&m_CommandActions);

		m_CommandActions.Bind(COMMAND_TOGGLE_FOCUS, [this] () {
			m_MovableController.Toggle();
			});

		m_CommandActions.Bind(COMMAND_INCREACE_SPEED, [this] () {
			m_pDebugSpectator->Increase();
			});

		m_CommandActions.Bind(COMMAND_DECREACE_SPEED, [this] () {
			m_pDebugSpectator->Decrease();
			});
	}

	void Register(NotNull<Nt::Window*> pWindow) override {
		m_Input.Register(pWindow);
		m_MovableController.Register(pWindow);
	}
	void Unregister(NotNull<Nt::Window*> pWindow) override {
		m_Input.Unregister(pWindow);
		m_MovableController.Unregister(pWindow);
	}

	void Update(const Float& deltaTime) override {
		m_MovableController.Update(deltaTime);
	}

private:
	DebugSpectator* m_pDebugSpectator;
	MovableController m_MovableController;
	InputMapper m_Input;
	CommandActions m_CommandActions;
};