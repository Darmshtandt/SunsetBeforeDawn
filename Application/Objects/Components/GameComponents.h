#pragma once

#include <Objects/Components/BasicComponents.h>

template <typename _Ty>
class GameResource {
public:
	GameResource() noexcept = default;
	virtual ~GameResource() noexcept = default;

	[[nodiscard]] const _Ty& GetAmount() const noexcept {
		return m_Amount;
	}
	[[nodiscard]] const _Ty& GetMaxAmount() const noexcept {
		return m_Amount;
	}

	void SetAmount(const _Ty& amount) noexcept {
		m_Amount = Nt::Clamp(amount, _Ty(0), m_MaxAmount);
	}
	void SetMaxAmount(const _Ty& amount) noexcept {
		m_Amount = Nt::Clamp(amount, _Ty(0), m_MaxAmount);
	}

protected:
	_Ty m_Amount;
	_Ty m_MaxAmount;
};

class Armor final : public IComponent, public GameResource<Float> {
public:
	explicit Armor(GameObject* pObject) : IComponent(pObject)
	{
	}
	~Armor() noexcept override = default;

	void TakeDamage(const Float& amount) noexcept {
		SetAmount(m_Amount - amount);
	}
	void Repair(const Float& amount) noexcept {
		SetAmount(m_Amount + amount);
	}

	void Break() noexcept {
		m_Amount = 0.f;
	}
	void Restore() noexcept {
		m_Amount = m_MaxAmount;
	}

	[[nodiscard]] Bool IsIntact() const noexcept {
		return m_Amount > 0;
	}
};

class Health final : public IComponent, public GameResource<Float> {
public:
	explicit Health(GameObject* pObject) : IComponent(pObject) {
	}
	~Health() noexcept override = default;

	void Drain(const Float& amount) noexcept {
		SetAmount(m_Amount - amount);
	}
	void Heal(const Float& amount) noexcept {
		SetAmount(m_Amount + amount);
	}

	void Kill() noexcept {
		m_Amount = 0.f;
	}
	void Revive() noexcept {
		m_Amount = m_MaxAmount;
	}

	[[nodiscard]] Bool IsAlive() const noexcept {
		return m_Amount > 0.f;
	}
};