#include <Objects/Components/IComponent.h>

IComponent::IComponent(const ComponentID& id) noexcept :
	Identifier(id)
{
}