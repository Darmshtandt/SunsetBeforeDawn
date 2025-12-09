#pragma once

#include <Objects/Weapons/IWeapon.h>

class Fist final : public IWeapon {
public:
	Fist() noexcept : IWeapon(Class<Fist>::ID())
	{
	}

	Fist(const Fist&) noexcept = default;
	Fist(Fist&&) noexcept = default;
	~Fist() noexcept override = default;

	Fist& operator = (const Fist&) noexcept = default;
	Fist& operator = (Fist&&) noexcept = default;
};
