#pragma once

#include <Controllers/IController.h>
#include <Objects/Actors/Player.h>
#include <Objects/Components/InputComponent.h>
#include <Objects/Components/MovementComponent.h>

class PlayerController final : public IController {
public:
	explicit PlayerController(NotNull<Player*> pPlayer);

	PlayerController(const PlayerController&) = delete;
	PlayerController(PlayerController&&) noexcept = default;
	~PlayerController() noexcept override = default;

	PlayerController& operator = (const PlayerController&) = delete;
	PlayerController& operator = (PlayerController&&) noexcept = default;

	void Update(const Float& deltaTime) override;

private:
	Player* m_pPlayer;
	InputComponent* m_pInputComponent;
	MovementComponent* m_pMovementComponent;

private:
	Nt::Float3D GetDirection() const noexcept;
};