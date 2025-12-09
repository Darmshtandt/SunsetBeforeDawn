#pragma once

#include <Core/EventBus.h>
#include <Engine/IController.h>
#include <World/Objects/GameObject.h>

class Combat;
class IWeapon;
class WeaponBelt;

class WeaponController : public IController {
public:
	explicit WeaponController(NotNull<GameObject*> pObject);

	WeaponController() = delete;
	WeaponController(const WeaponController&) noexcept = default;
	WeaponController(WeaponController&&) noexcept = default;
	~WeaponController() noexcept override;

	WeaponController& operator = (const WeaponController&) noexcept = default;
	WeaponController& operator = (WeaponController&&) noexcept = default;

	void Update(const Float& deltaTime) override;

	void Execute(const std::unordered_set<InputCommandID>& commands);

private:
	Subscription m_ActionSubscription;
	Combat* m_pCombat;
	WeaponBelt* m_pWeaponBelt;
	const IWeapon* m_pSelectedWeapon;
};