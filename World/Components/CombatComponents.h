#pragma once

#include <Engine/Combat/CombatInterfaces.h>
#include <World/Objects/Weapons/IWeapon.h>

class WeaponBelt final : public IComponent {
public:
	explicit WeaponBelt(GameObject* pObject) :
		IComponent(pObject, Class<WeaponBelt>::ID())
	{
	}
	~WeaponBelt() noexcept override = default;

	template <class _Ty> requires std::is_base_of_v<IWeapon, _Ty>
	_Ty* Add() {
		constexpr ClassID id = Class<_Ty>::ID();
		if (!m_WeaponMap.contains(id))
			m_WeaponMap[id] = std::make_unique<_Ty>();
		return static_cast<_Ty*>(m_WeaponMap[id].get());
	}
	template <class _Ty> requires std::is_base_of_v<IWeapon, _Ty>
	void Remove() {
		constexpr ClassID id = Class<_Ty>::ID();
		const auto it = m_WeaponMap.find(id);
		if (it != m_WeaponMap.cend())
			m_WeaponMap.erase(it);
	}
	void Clear() noexcept {
		m_WeaponMap.clear();
	}

	[[nodiscard]] const IWeapon* GetFirst() const noexcept {
		if (m_WeaponMap.empty())
			return nullptr;
		return m_WeaponMap.begin()->second.get();
	}
	[[nodiscard]] const IWeapon* GetNext(const IWeapon* pWeapon) const noexcept {
		if (pWeapon == nullptr)
			return GetFirst();

		auto it = m_WeaponMap.find(pWeapon->GetID());
		if (it == m_WeaponMap.cend())
			return nullptr;

		if (++it == m_WeaponMap.cend())
			return m_WeaponMap.begin()->second.get();
		return it->second.get();
	}

private:
	std::unordered_map<ClassID, std::unique_ptr<IWeapon>> m_WeaponMap;
};

class Combat final : public IComponent {
public:
	explicit Combat(GameObject* pObject) :
		IComponent(pObject, Class<Combat>::ID())
	{
	}
	~Combat() noexcept override = default;

	void PerformAttack() {
		if (m_Timer.GetElapsedTimeMs() < m_DelayMs)
			return;

		m_Timer.Restart();

		Assert(m_pDamage != nullptr, "Damage not selected");
		if (m_pCombatDispatcher == nullptr)
			return;

		DamageCommand command;
		command.pDamage = m_pDamage;
		command.pOwner = OwnerPtr();
		command.pOwnerTransform = RequireNotNull(command.pOwner->GetComponent<Transform3D>());
		m_pCombatDispatcher->Dispatch(command);
	}

	void SetDelayMs(const uInt& delayMs) noexcept {
		m_DelayMs = delayMs;
	}
	void SetDamageType(const IDamage* pDamage) noexcept {
		m_pDamage = pDamage;
	}
	void SetCombatDispatcher(ICombatDispatcher* pDispatcher) noexcept {
		m_pCombatDispatcher = pDispatcher;
	}

private:
	const IDamage* m_pDamage = nullptr;
	ICombatDispatcher* m_pCombatDispatcher = nullptr;
	Nt::Timer m_Timer;
	uInt m_DelayMs = 1000;
};