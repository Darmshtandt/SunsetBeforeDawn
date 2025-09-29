#pragma once

#include <ClassManager.h>
#include <Nt/Collider.h>
#include <Nt/Graphics/Geometry/Shape.h>
#include <Objects/GameObject.h>

class IBody : public Identifier {
public:
	IBody(NotNull<GameObject*> pOwner, const ClassID& id) noexcept;

	IBody() = delete;
	IBody(const IBody&) noexcept = default;
	IBody(IBody&&) noexcept = default;
	~IBody() noexcept override = default;

	IBody& operator = (const IBody&) noexcept = default;
	IBody& operator = (IBody&&) noexcept = default;

	virtual void Update(const Float& deltaTime) = 0;
	virtual Bool HandleCollision(NotNull<const IBody*> pOther) = 0;

	[[nodiscard]] GameObject* GetOwnerPtr() noexcept;
	[[nodiscard]] Nt::Collider& GetCollider() noexcept;
	[[nodiscard]] const Nt::Collider& GetCollider() const noexcept;

	void SetColliderShape(const Nt::Shape& shape);

protected:
	GameObject* m_pOwner;
	Nt::Collider m_Collider;
};