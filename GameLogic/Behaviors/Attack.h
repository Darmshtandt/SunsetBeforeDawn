#pragma once

#include <Nt/Core/Math/Vector.h>
#include <Engine/AI/IBehavior.h>

class Attack final : public IBehavior {
public:
	Attack() noexcept;
	Attack(const Attack&) noexcept = default;
	Attack(Attack&&) noexcept = default;
	~Attack() noexcept override = default;

	Attack& operator = (const Attack&) noexcept = default;
	Attack& operator = (Attack&&) noexcept = default;

	void Update(Actor& actor, const Float& deltaTime) override;

private:
	inline static BehaviorRegistrar<Attack> m_Registrar;
};