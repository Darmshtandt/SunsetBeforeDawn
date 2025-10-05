#pragma once

#include <Controllers/IController.h>
#include <Objects/Actors/Player.h>

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
	Transform3D* m_pTransform;
	Movement3D* m_pMovement;
	InputComponent* m_pInput;

private:
	Nt::Float3D GetDirection() const noexcept;
};