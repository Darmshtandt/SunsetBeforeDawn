#include <Objects/Interactives/Interactive.h>

Interactive::Interactive(const ClassID& id) noexcept :
	GameObject(id, ObjectType::Interactive)
{
}