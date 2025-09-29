#pragma once

#include <Objects/Actors/Character.h>

class Player final : public Character {
public:
	Player() noexcept;
	Player(const Player&) noexcept = default;
	Player(Player&&) noexcept = default;
	~Player() noexcept override = default;

	Player& operator = (const Player&) noexcept = default;
	Player& operator = (Player&&) noexcept = default;

private:
	inline static ObjectRegistrar<Player> m_Registrar = { "Player" };
};

using PlayerPtr = std::shared_ptr<Player>;