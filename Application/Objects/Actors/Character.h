#pragma once

#include <Objects/Actors/Movable.h>
#include <Objects/Actors/Actor.h>
#include <Objects/GameObject.h>

class Character : public GameObject, public Movable, public Actor {
protected:
	Character(const ClassID& id, const ObjectType& type, std::string factionName) noexcept;

public:
	Character() = delete;
	Character(const Character&) noexcept = default;
	Character(Character&&) noexcept = default;
	~Character() noexcept override = default;

	Character& operator = (const Character&) noexcept = default;
	Character& operator = (Character&&) noexcept = default;

	void Move(const Nt::Float3D& velocity) override;
	void ApplyRotation(const Nt::Float3D& angle) override;

	[[nodiscard]] Float GetSpeed() const noexcept override;
	[[nodiscard]] Float GetPitch() const noexcept override;
	[[nodiscard]] std::string GetFactionName() const noexcept;

	void SetSpeed(const Float& speed) noexcept;

private:
	Float m_Speed = 5.f;
	std::string m_FactionName;
};