#pragma once

#include <Signal.h>
#include <Objects/Actors/Character.h>
#include <Objects/NPC/Behaviors/IBehavior.h>

class MeshComponent;
class TextureComponent;
class Creature;

struct MovableCreature final {
	const Transform3D* pTransform = nullptr;
	Movement3D* pMovement = nullptr;
	Creature* pCreature = nullptr;
};

class Creature : public Character {
public:
	struct Target final {
		Transform3D* pTransform = nullptr;
		GameObject* pObject = nullptr;
	};

	struct View final {
		Float RangeSq = 0.f;
		Float Angle = 0.f;
	};

protected:
	Creature(const ClassID& id, std::string factionName);

public:
	Creature() = delete;
	Creature(const Creature&) noexcept = default;
	Creature(Creature&&) noexcept = default;
	~Creature() noexcept override = default;

	Creature& operator = (const Creature&) noexcept = default;
	Creature& operator = (Creature&&) noexcept = default;

	void Update(const Float& deltaTime) override;

	void SetTarget(const Target& target);
	void SetBehavior(NotNull<IBehavior*> pBehavior) noexcept;
	void SetOnTargetFound(const Signal<Creature&>::Slot& slot);
	void SetOnTargetLost(const Signal<Creature&>::Slot& slot);

	[[nodiscard]] Float GetPersecutionDistance() const noexcept;
	[[nodiscard]] const Target& GetTarget() const noexcept;
	[[nodiscard]] const View& GetView() const noexcept;
	[[nodiscard]] Bool HasTarget() const noexcept;
	[[nodiscard]] Bool CanInteract() const noexcept;

protected:
	Transform3D* m_pTransform = nullptr;
	Movement3D* m_pMovement = nullptr;
	MeshComponent* m_pMesh = nullptr;
	TextureComponent* m_pTexture = nullptr;
	View m_View;
	Float m_InteractionDistSq = 0.f;
	Float m_PersecutionDistSq = 0.f;

private:
	MovableCreature m_Movable;
	Signal<Creature&> m_OnTargetFound;
	Signal<Creature&> m_OnTargetLost;
	Target m_Target;
	IBehavior* m_pBehavior = nullptr;
};

using CreaturePtr = std::shared_ptr<Creature>;