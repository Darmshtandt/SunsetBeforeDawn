#pragma once

#include <unordered_set>
#include <Engine/IController.h>
#include <World/Objects/Player.h>
#include <Core/EventBus.h>

enum class InputCommandID : Byte;

class PlayerController final : public IController {
public:
	explicit PlayerController(NotNull<Player*> pPlayer);

	PlayerController(const PlayerController&) = delete;
	PlayerController(PlayerController&&) noexcept = default;
	~PlayerController() override;

	PlayerController& operator = (const PlayerController&) = delete;
	PlayerController& operator = (PlayerController&&) noexcept = default;

	void Update(const Float& deltaTime) override{}

private:
	Subscription m_ActionSubscription;
	Transform3D* m_pTransform;
	Movement3D* m_pMovement;

private:
	void Execute(const std::unordered_set<InputCommandID>& commands) const noexcept;
};
