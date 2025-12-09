#pragma once

#include <Engine/Combat/DamageLogic.h>
#include <World/Objects/Weapons/IWeapon.h>

class Pistol : public IWeapon {
public:
	Pistol() noexcept : IWeapon(Class<Pistol>::ID()) {
		m_Damage.Origin.y = 0.75f;
		m_Damage.Distance = 1000;
		m_Damage.Amount = 10;
	}

	[[nodiscard]]
	const IDamage* GetDamageType() const override {
		return &m_Damage;
	}

private:
	RayDamage m_Damage;
};
