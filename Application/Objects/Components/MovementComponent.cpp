#include <Objects/Components/MovementComponent.h>

MovementComponent::MovementComponent() noexcept :
	IComponent(Class<MovementComponent>::ID())
{
}