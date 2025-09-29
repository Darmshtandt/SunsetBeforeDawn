#pragma once

#include <Nt/Graphics/Objects/Model.h>
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

	[[nodiscard]] Float GetDistance(const GameObject& object) const noexcept;
	[[nodiscard]] ObjectType GetType() const noexcept;
	[[nodiscard]] IBody* GetBodyPtr() noexcept;
	[[nodiscard]] const IBody* GetBodyPtr() const noexcept;

protected:
	Signal<GameObject*> m_OnMoved;
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