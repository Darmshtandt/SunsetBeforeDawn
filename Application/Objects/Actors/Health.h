#pragma once

#include <Objects/Actors/Resource.h>


class Health : public Resource<Float> {
public:
	using Resource::Resource;

	Void Drain(const Float& amount) noexcept {
		Decrease(amount);
	}
	Void Heal(const Float& amount) noexcept {
		Increase(amount);
	}

	Void Kill() noexcept {
		Clear();
	}
	Void Revive() noexcept {
		Reset();
	}

	[[nodiscard]]
	Bool IsAlive() const noexcept {
		return CanUse();
	}

private:
	using Resource::Decrease;
	using Resource::Increase;
	using Resource::Clear;
	using Resource::Reset;
	using Resource::CanUse;
};