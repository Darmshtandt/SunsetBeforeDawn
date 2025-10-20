#pragma once

#include <World/Components/IComponent.h>
#include <Core/QueryBus.h>
#include <Core/RegistrarBase.h>

enum class ObjectType : Byte {
	Debug, Static,
	Player, Creature, Interactive,
	Canvas, UI
};

class GameObject : public Identifier {
protected:
	GameObject(const ClassID& id, const ObjectType& type) noexcept;

public:
	GameObject() = delete;
	GameObject(const GameObject&) = delete;
	GameObject(GameObject&&) noexcept = default;
	~GameObject() noexcept override = default;

	GameObject& operator = (const GameObject&) = delete;
	GameObject& operator = (GameObject&&) noexcept = default;

	template <class _Ty>
	_Ty* AddComponent() {
		const ClassID id = Class<_Ty>::ID();
		if (!m_Components.contains(id))
			m_Components[id] = std::make_unique<_Ty>(this);
		return static_cast<_Ty*>(m_Components[id].get());
	}

	template <class _Ty>
	void RemoveComponent() {
		const ClassID id = Class<_Ty>::ID();
		Assert(m_Components[id], "Does not have a component");
		m_Components.erase(id);
	}

	template <class _Ty>
	[[nodiscard]] const _Ty* GetComponent() const noexcept {
		return _GetComponentImpl<_Ty>();
	}

	template <class _Ty>
	[[nodiscard]] _Ty* GetComponent() noexcept {
		return const_cast<_Ty*>(_GetComponentImpl<_Ty>());
	}

	template <class _Ty>
	[[nodiscard]] Bool HasComponent() const noexcept {
		return m_Components.contains(Class<_Ty>::ID());
	}

	[[nodiscard]] ObjectType GetType() const noexcept;

protected:
	std::unordered_map<ClassID, std::unique_ptr<IComponent>> m_Components;
	ObjectType m_Type;

private:
	template <class _Ty>
	[[nodiscard]] const _Ty* _GetComponentImpl() const noexcept {
		const auto it = m_Components.find(Class<_Ty>::ID());
		if (it == m_Components.cend())
			return nullptr;
		return static_cast<const _Ty*>(it->second.get());
	}
};

using ObjectPtr = std::shared_ptr<GameObject>;
using ObjectFactory = FactoryBase<std::shared_ptr, GameObject, ObjectPtr(*)()>;

template <class _Ty>
using ObjectRegistrar = RegistrarBase<std::shared_ptr, _Ty, ObjectFactory>;

#include <World/Components/BasicComponents.h>