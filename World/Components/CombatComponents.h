#pragma once

#include <Engine/Combat/CombatInterfaces.h>
#include <World/Objects/Weapons/IWeapon.h>

class WeaponBelt final : public IComponent {
public:
	explicit WeaponBelt(GameObject* pObject) : IComponent(pObject) {
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
	explicit Combat(GameObject* pObject) : IComponent(pObject)
	{
	}
	~Combat() noexcept override = default;

	void PerformAttack(const Float& direction) {
		if (m_pAttackDispatcher == nullptr)
			return;

		AttackCommand command = { };
		command.pObject = OwnerPtr();
		command.pStrategy = m_pAttackStrategy;
		command.Direction = direction;

		m_pAttackDispatcher->Dispatch(command);
	}

	void SetAttackStrategy(IAttackStrategy* pStrategy) {
		m_pAttackStrategy = pStrategy;
	}
	void SetAttackDispatcher(IAttackDispatcher* pDispatcher) {
		m_pAttackDispatcher = pDispatcher;
	}

private:
	IAttackStrategy* m_pAttackStrategy = nullptr;
	IAttackDispatcher* m_pAttackDispatcher = nullptr;
};