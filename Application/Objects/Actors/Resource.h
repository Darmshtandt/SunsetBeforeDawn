#pragma once

#include <Nt/Core/Math/Math.h>

template <typename _Ty>
class Resource {
private:
	static constexpr _Ty m_Epsilon = std::numeric_limits<_Ty>::epsilon();

public:
	Resource(const _Ty& amount, const _Ty& maxAmount) :
		m_Amount(amount),
		m_MaxAmount(maxAmount)
	{
	}

	Void Decrease(const _Ty& amount) noexcept {
		if (amount > m_Epsilon)
			SetAmount(m_Amount - amount);
	}
	Void Increase(const _Ty& amount) noexcept {
		if (amount > m_Epsilon)
			SetAmount(m_Amount + amount);
	}

	Void Clear() noexcept {
		m_Amount = m_Epsilon;
	}
	Void Reset() noexcept {
		m_Amount = m_MaxAmount;
	}

	[[nodiscard]]
	const _Ty& GetAmount() const noexcept {
		return m_Amount;
	}

	[[nodiscard]]
	Bool CanUse() const noexcept {
		return (m_Amount > m_Epsilon);
	}

	Void SetAmount(const _Ty& amount) noexcept {
		m_Amount = Nt::Clamp(amount, m_Epsilon, m_MaxAmount);
	}

private:
	_Ty m_Amount;
	_Ty m_MaxAmount;
};