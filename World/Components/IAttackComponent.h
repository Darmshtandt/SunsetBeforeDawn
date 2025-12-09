#pragma once

#include <World/Objects/GameObject.h>

class IAttackComponent {
protected:
	explicit IAttackComponent(GameObject& object) noexcept :
		pSender(&object)
	{
	}

public:
	virtual ~IAttackComponent() noexcept = default;

	GameObject* pSender;
};