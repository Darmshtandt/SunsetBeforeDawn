#include <Physics/StaticBody.h>

StaticBody::StaticBody(const NotNull<GameObject*>& pOwner):
	IBody(pOwner, Class<StaticBody>::ID())
{
}

void StaticBody::Update(const Float& deltaTime) {
	(void)deltaTime;
}

Bool StaticBody::HandleCollision(NotNull<const IBody*> pOther) {
	(void)pOther;
	return false;
}