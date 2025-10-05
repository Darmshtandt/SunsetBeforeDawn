#include <Objects/NPC/Creature.h>
#include <Objects/Components/RenderComponents.h>

Creature::Creature(const ClassID& id, std::string factionName) :
	Character(id, ObjectType::Creature, std::move(factionName))
{
	m_pTransform = AddComponent<Transform3D>();
	m_pMesh = AddComponent<MeshComponent>();
	m_pTexture = AddComponent<TextureComponent>();
	m_pMovement = AddComponent<Movement3D>();

	m_Movable.pTransform = m_pTransform;
	m_Movable.pMovement = m_pMovement;
	m_Movable.pCreature = this;
}

void Creature::Update(const Float& deltaTime) {
	if (HasTarget()) {
		const Float distanceSqToTarget =
			m_Movable.pTransform->DistanceSquare(*m_Target.pTransform);

		if (distanceSqToTarget >= m_PersecutionDistSq) {
			m_OnTargetLost.Emmit(*this);
			m_Target = { };
		}
	}

	RequireNotNull(m_pBehavior)->Update(m_Movable, deltaTime);
}

void Creature::SetTarget(const Target& target) {
	const Bool hasChanged = m_Target.pObject != target.pObject;

	if (m_Target.pObject != nullptr)
		RequireNotNull(target.pTransform);

	m_Target = target;
	if (!hasChanged)
		return;

	if (m_Target.pObject == nullptr)
		m_OnTargetLost.Emmit(*this);
	else
		m_OnTargetFound.Emmit(*this);
}

void Creature::SetBehavior(NotNull<IBehavior*> pBehavior) noexcept {
	m_pBehavior = pBehavior;
}
void Creature::SetOnTargetFound(const Signal<Creature&>::Slot& slot) {
	m_OnTargetFound.Connect(slot);
}
void Creature::SetOnTargetLost(const Signal<Creature&>::Slot& slot) {
	m_OnTargetLost.Connect(slot);
}

Float Creature::GetPersecutionDistance() const noexcept {
	return m_PersecutionDistSq;
}

const Creature::Target& Creature::GetTarget() const noexcept {
	return m_Target;
}

const Creature::View& Creature::GetView() const noexcept {
	return m_View;
}

Bool Creature::HasTarget() const noexcept {
	return m_Target.pObject != nullptr;
}

Bool Creature::CanInteract() const noexcept {
	if (m_Target.pObject == nullptr)
		return false;

	const Float distanceSqToTarget = 
		m_Movable.pTransform->DistanceSquare(*m_Target.pTransform);
	return distanceSqToTarget <= m_InteractionDistSq;
}