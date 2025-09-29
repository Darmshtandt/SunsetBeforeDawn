#pragma once

#include <Nt/IBody.h>

class StaticBody : public IBody {
public:
	explicit StaticBody(const NotNull<GameObject*>& pOwner);

	StaticBody() = delete;
	StaticBody(const StaticBody& body) noexcept = default;
	StaticBody(StaticBody&& body) noexcept = default;
	~StaticBody() noexcept override = default;

	StaticBody& operator = (const StaticBody&) noexcept = default;
	StaticBody& operator = (StaticBody&&) noexcept = default;

	void Update(const Float& deltaTime) override;
	Bool HandleCollision(NotNull<const IBody*> pOther) override;
};
