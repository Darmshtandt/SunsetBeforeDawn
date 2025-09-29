#pragma once

#include <Nt/IBody.h>

class CollisionSystem final {
public:
	CollisionSystem() noexcept = default;
	CollisionSystem(const CollisionSystem&) noexcept = default;
	CollisionSystem(CollisionSystem&&) noexcept = default;
	~CollisionSystem() noexcept = default;

	CollisionSystem& operator = (const CollisionSystem&) noexcept = default;
	CollisionSystem& operator = (CollisionSystem&&) noexcept = default;

	void Update(const Float& deltaTime);
};