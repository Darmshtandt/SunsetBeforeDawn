#pragma once

#include <Nt/Core/NtTypes.h>

struct IAlive {
	virtual void Drain(const Float& amount) noexcept = 0;
	virtual void Heal(const Float& amount) noexcept = 0;

	virtual void Kill() noexcept = 0;
	virtual void Revive() noexcept = 0;

	[[nodiscard]] virtual Float GetHealth() const noexcept = 0;
	virtual void SetHealth(const Float& amount) noexcept = 0;
};