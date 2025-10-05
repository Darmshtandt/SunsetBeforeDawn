#pragma once

#include <Nt/Core/Utilities.h>

class GameObject;

class IComponent {
public:
	explicit IComponent(NotNull<GameObject*> pOwner) noexcept :
		m_pOwner(pOwner) {
	}
	virtual ~IComponent() noexcept = default;

	[[nodiscard]]
	GameObject* OwnerPtr() noexcept {
		return m_pOwner;
	}

private:
	GameObject* m_pOwner;
};