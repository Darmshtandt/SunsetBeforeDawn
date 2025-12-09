#pragma once

#include <Nt/Core/NtTypes.h>

class GameObject;

struct ISystem {
	virtual ~ISystem() noexcept = default;

	virtual void RegisterObject(GameObject& object) = 0;
	virtual void UnregisterObject(const GameObject& object) = 0;

	virtual void Update(const Float& deltaTime) = 0;
};

using SystemPtr = std::unique_ptr<ISystem>;