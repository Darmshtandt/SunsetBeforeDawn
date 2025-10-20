#pragma once

#include <Engine/IController.h>
#include <World/Objects/GameObject.h>

class IWeapon;
class WeaponBelt;

class WeaponController : public IController {
public:
	explicit WeaponController(const GameObject& object);

	WeaponController() = delete;
	WeaponController(const WeaponController&) noexcept = default;
	WeaponController(WeaponController&&) noexcept = default;
	~WeaponController() noexcept override = default;

	WeaponController& operator = (const WeaponController&) noexcept = default;
	WeaponController& operator = (WeaponController&&) noexcept = default;

	void Update(const Float& deltaTime) override;

	void Execute(const std::unordered_set<InputCommandID>& commands);

private:
	const WeaponBelt* m_pWeaponBelt;
	const IWeapon* m_pSelectedWeapon;
};