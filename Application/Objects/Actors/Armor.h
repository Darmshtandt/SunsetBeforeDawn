#pragma once

#include <Objects/Actors/Resource.h>

class Armor : public Resource<Float> {
public:
	using Resource::Resource;

	Void TakeDamage(const Float& amount) noexcept {
		Decrease(amount);
	}
	Void Repair(const Float& amount) noexcept {
		Increase(amount);
	}

	Void Break() noexcept {
		Clear();
	}
	Void Restore() noexcept {
		Reset();
	}

	[[nodiscard]]
	Bool IsIntact() const noexcept {
		return CanUse();
	}

private:
	using Resource::Decrease;
	using Resource::Increase;
	using Resource::Clear;
	using Resource::Reset;
	using Resource::CanUse;
};