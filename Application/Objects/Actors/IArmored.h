#pragma once

#include <Nt/Core/NtTypes.h>

struct IArmored {
	[[nodiscard]] virtual Float GetArmor() const noexcept = 0;
	virtual void SetArmor(const Float& amount) noexcept = 0;
};