#pragma once

#include <GameLogic/Behaviors/BehaviorLocator.h>

template <class _Ty>
class BehaviorRegistrar final {
public:
	BehaviorRegistrar() {
		BehaviorLocator::Register<_Ty>();
	}
};