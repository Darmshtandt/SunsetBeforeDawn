#pragma once

#include <Objects/GameObject.h>

struct IAction {
	virtual void Execute(const Double& deltaTime) = 0;

	[[nodiscard]]
	virtual GameObject* GetObjectPtr() = 0;

	[[nodiscard]]
	virtual Bool IsDone() const noexcept = 0;

	virtual void SetDone() = 0;
};

using ActionPtr = std::shared_ptr<IAction>;