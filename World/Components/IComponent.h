#pragma once

#include <Nt/Core/Utilities.h>
#include <Core/Nt/ClassManager.h>

class GameObject;

class IComponent : public Identifier {
public:
	explicit IComponent(NotNull<GameObject*> pOwner, ClassID id) noexcept :
		m_pOwner(pOwner),
		Identifier(id)
	{
	}
	virtual ~IComponent() noexcept = default;

	[[nodiscard]] GameObject* OwnerPtr() noexcept {
		return m_pOwner;
	}

private:
	GameObject* m_pOwner;
};