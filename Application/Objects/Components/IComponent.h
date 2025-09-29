#pragma once

#include <ClassManager.h>

using ComponentID = ClassID;

class IComponent : public Identifier {
protected:
	explicit IComponent(const ComponentID& id) noexcept;

public:
	IComponent() = delete;
	IComponent(const IComponent&) noexcept = default;
	IComponent(IComponent&&) noexcept = default;
	~IComponent() noexcept override = default;

	IComponent& operator = (const IComponent&) noexcept = default;
	IComponent& operator = (IComponent&&) noexcept = default;
};