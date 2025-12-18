#pragma once

#include <World/Components/RenderComponents.h>
#include <World/Objects/Character.h>

class Player final : public Character {
public:
	struct Statistics final {
		Float HealthAmount = -FLT_MAX;
		Float ArmorAmount = -FLT_MAX;
		Float HealthMaxAmount = -FLT_MAX;
		Float ArmorMaxAmount = -FLT_MAX;
	};

public:
	Player();
	Player(const Player&) noexcept = default;
	Player(Player&&) noexcept = default;
	~Player() noexcept override = default;

	Player& operator = (const Player&) noexcept = default;
	Player& operator = (Player&&) noexcept = default;

	[[nodiscard]] Nt::Float3D GetFullRotation() const noexcept;

	[[nodiscard]] Statistics GetStatistics() const noexcept;

private:
	Camera3D* m_pCamera;
	inline static ObjectRegistrar<Player> m_Registrar = { "Player" };
};

using PlayerPtr = std::shared_ptr<Player>;