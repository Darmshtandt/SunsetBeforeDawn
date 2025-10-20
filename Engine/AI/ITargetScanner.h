#pragma once

#include <World/Entities.h>

struct ITargetScanner {
	using Target = Intent3D::Target;
	using View = Intent3D::View;

	virtual ~ITargetScanner() noexcept = default;

	[[nodiscard]] virtual 
	Bool IsVisibleTarget(const Pawn& pawn, const View& view, const Target& target) const noexcept = 0;

	[[nodiscard]] virtual 
	Bool Scan(Pawn& pawn, const std::vector<Target>& targets, const View& view) = 0;
};