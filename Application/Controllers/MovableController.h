#pragma once

#include <Objects/Actors/Movable.h>
#include <Controllers/IController.h>
#include <Controllers/InputMapper.h>

class MovableController : public IController {
public:
	using enum Nt::Key;

	static constexpr const Char* COMMAND_LEFT = "Left";
	static constexpr const Char* COMMAND_RIGHT = "Right";
	static constexpr const Char* COMMAND_UP = "Up";
	static constexpr const Char* COMMAND_DOWN = "Down";
	static constexpr const Char* COMMAND_FORWARD = "Forward";
	static constexpr const Char* COMMAND_BACK = "Back";

public:
	explicit MovableController(NotNull<Movable*> pMovable) :
		m_pMovable(pMovable)
	{
		m_Input.BindKeyCommand(_W, COMMAND_FORWARD);
		m_Input.BindKeyCommand(_A, COMMAND_LEFT);
		m_Input.BindKeyCommand(_S, COMMAND_BACK);
		m_Input.BindKeyCommand(_D, COMMAND_RIGHT);
		m_Input.BindKeyCommand(_SPACE, COMMAND_UP);
		m_Input.BindKeyCommand(_SHIFT, COMMAND_DOWN);
	}

	void Register(NotNull<Nt::Window*> pWindow) override {
		m_Input.Register(pWindow);
	}
	void Unregister(NotNull<Nt::Window*> pWindow) override {
		m_Input.Unregister(pWindow);
	}

	void Update(const Float& deltaTime) override {
		if (!IsEnabled() || !m_Input.HasActive())
			return;

		const Nt::Float3D direction = GetDirection();
		if (direction.LengthSquare() == 0.f) {
			if (m_IsMove) {
				m_IsMove = false;
				m_pMovable->StopMoving();
			}

			return;
		}

		if (!m_IsMove) {
			m_IsMove = true;
			m_pMovable->StartMoving();
		}

		m_pMovable->Move(direction * m_pMovable->GetSpeed() * deltaTime);
	}

private:
	InputMapper m_Input;
	Movable* m_pMovable;
	Bool m_IsMove = false;

private:
	Nt::Float3D GetDirection() const noexcept {
		const Float& pitch = m_pMovable->GetPitch();

		Nt::Float3D direction;
		if (m_Input.IsActive(COMMAND_LEFT)) {
			direction.x += sinf((pitch - 90.f) * RADf);
			direction.z += cosf((pitch - 90.f) * RADf);
		}
		if (m_Input.IsActive(COMMAND_RIGHT)) {
			direction.x += sinf((pitch + 90.f) * RADf);
			direction.z += cosf((pitch + 90.f) * RADf);
		}

		if (m_Input.IsActive(COMMAND_UP))
			direction.y += 1.f;
		if (m_Input.IsActive(COMMAND_DOWN))
			direction.y -= 1.f;

		if (m_Input.IsActive(COMMAND_FORWARD)) {
			direction.x -= sinf(pitch * RADf);
			direction.z -= cosf(pitch * RADf);
		}
		if (m_Input.IsActive(COMMAND_BACK)) {
			direction.x += sinf(pitch * RADf);
			direction.z += cosf(pitch * RADf);
		}

		return direction.GetNormalize();
	}
};