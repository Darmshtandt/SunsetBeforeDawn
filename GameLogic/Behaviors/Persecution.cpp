#include <GameLogic/Behaviors/Persecution.h>
#include <World/Objects/Creature.h>

Persecution::Persecution() noexcept :
	IBehavior(Class<Persecution>::ID())
{
}

void Persecution::Update(Actor& actor, const Float& deltaTime) {
	if (!actor.pRoute->HasMarker())
		return;

	if (actor.pRoute->IsReachedMarker(deltaTime)) {
		actor.pRoute->PopMarker();
		if (!actor.pRoute->HasMarker())
			return;
	}

	const Nt::Float3D direction = -actor.pRoute->DirectionToMarker();

	actor.pMovement->Direction = direction;
	actor.pMovement->DesiredRotation.y = std::atan2(direction.x, direction.z);
	actor.pMovement->DesiredRotation.y -= actor.pTransform->Rotation().y;
	actor.pMovement->DesiredRotation.y /= deltaTime * 4.f;

	if (!std::isfinite(actor.pMovement->DesiredRotation.y))
		actor.pMovement->DesiredRotation.y = 0.f;
}