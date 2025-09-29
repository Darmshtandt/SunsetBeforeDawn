#pragma once

#include <Nt/Graphics/Objects/Model.h>
#include <Objects/Components/IComponent.h>
#include <RegistrarBase.h>
#include <Signal.h>

class IBody;

enum class ObjectType : Byte {
	Debug,
	Player,
	Creature,
	Interactive,
	Static
};

class GameObject : public Nt::Model, public Identifier {
private:
	using ComponentPtr = std::unique_ptr<IComponent>;

protected:
	GameObject(const ClassID& id, const ObjectType& type) noexcept;

public:
	GameObject() = delete;
	GameObject(const GameObject&) noexcept = default;
	GameObject(GameObject&&) noexcept = default;
	~GameObject() noexcept override = default;

	GameObject& operator = (const GameObject&) noexcept = default;
	GameObject& operator = (GameObject&&) noexcept = default;

	void Render(NotNull<Nt::Renderer*> pRenderer) const override;
	void Translate(const Nt::Float3D& offset) noexcept;
	void SubscribeOnMoved(const Signal<GameObject*>::Slot& onMoved);

	template <class _Ty> requires std::is_base_of_v<IComponent, _Ty>
	void AddComponent() {
		m_Components[Class<_Ty>::ID()] = std::make_unique<_Ty>();
	}

	template <class _Ty> requires std::is_base_of_v<IComponent, _Ty>
	[[nodiscard]] const _Ty* GetComponent() const noexcept {
		auto it = m_Components.find(Class<_Ty>::ID());
		if (it == m_Components.cend())
			return nullptr;
		return static_cast<const _Ty*>(it->second.get());
	}

	template <class _Ty> requires std::is_base_of_v<IComponent, _Ty>
	[[nodiscard]] _Ty* GetComponent() noexcept {
		auto it = m_Components.find(Class<_Ty>::ID());
		if (it == m_Components.cend())
			return nullptr;
		return static_cast<_Ty*>(it->second.get());
	}

	template <class _Ty> requires std::is_base_of_v<IComponent, _Ty>
	[[nodiscard]] Bool HasComponent() const noexcept {
		return m_Components.contains(Class<_Ty>::ID());
	}

	[[nodiscard]] Float GetDistance(const GameObject& object) const noexcept;
	[[nodiscard]] ObjectType GetType() const noexcept;
	[[nodiscard]] IBody* GetBodyPtr() noexcept;
	[[nodiscard]] const IBody* GetBodyPtr() const noexcept;

protected:
	Signal<GameObject*> m_OnMoved;
	std::unordered_map<ComponentID, ComponentPtr> m_Components;
	std::unique_ptr<IBody> m_pBody;
	ObjectType m_Type;

protected:
	template <class _Ty> requires std::is_base_of_v<IBody, _Ty>
	void SetBody() {
		m_pBody = std::make_unique<_Ty>(this);
	}
};

using ObjectPtr = std::shared_ptr<GameObject>;
using ObjectFactory = FactoryBase<std::shared_ptr, GameObject, ObjectPtr(*)()>;

template <class _Ty>
using ObjectRegistrar = RegistrarBase<std::shared_ptr, _Ty, ObjectFactory>;

#include <Nt/IBody.h>