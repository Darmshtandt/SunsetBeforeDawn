#pragma once

#include <Core/Nt/ClassManager.h>

class IWeapon : public Identifier {
protected:
	explicit IWeapon(const ClassID& id) noexcept : Identifier(id)
	{
	}

public:
	IWeapon() = delete;
	IWeapon(const IWeapon&) noexcept = default;
	IWeapon(IWeapon&&) noexcept = default;
	~IWeapon() noexcept override = default;

	IWeapon& operator = (const IWeapon&) noexcept = default;
	IWeapon& operator = (IWeapon&&) noexcept = default;

	[[nodiscard]]
	virtual const IDamage* GetDamageType() const = 0;
};