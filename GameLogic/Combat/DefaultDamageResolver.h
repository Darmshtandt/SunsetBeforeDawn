#pragma once

#include <Engine/Combat/CombatInterfaces.h>

class DefaultDamageResolver final : public IDamageResolver {
public:
	~DefaultDamageResolver() noexcept override = default;

	void Resolve() override;
};