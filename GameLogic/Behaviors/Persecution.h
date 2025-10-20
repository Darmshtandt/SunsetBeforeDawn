#pragma once

#include <Nt/Core/Math/Vector.h>
#include <Engine/AI/IBehavior.h>

class Persecution final : public IBehavior {
public:
	Persecution() noexcept;
	Persecution(const Persecution&) noexcept = default;
	Persecution(Persecution&&) noexcept = default;
	~Persecution() noexcept override = default;

	Persecution& operator = (const Persecution&) noexcept = default;
	Persecution& operator = (Persecution&&) noexcept = default;

	void Update(Actor& actor, const Float& deltaTime) override;

private:
	inline static BehaviorRegistrar<Persecution> m_Registrar;
};