#pragma once

#include <Core/EventBus.h>
#include <Core/QueryBus.h>

struct BusLocator final {
	[[nodiscard]] static EventBus& ActionDispatcher() noexcept {
		static EventBus bus;
		return bus;
	}
	[[nodiscard]] static EventBus& DebugDispatcher() noexcept {
		static EventBus bus;
		return bus;
	}
	[[nodiscard]] static EventBus& SceneDispatcher() noexcept {
		static EventBus bus;
		return bus;
	}
	[[nodiscard]] static EventBus& ObjectDispatcher() noexcept {
		static EventBus bus;
		return bus;
	}
	[[nodiscard]] static EventBus& PhysicsDispatcher() noexcept {
		static EventBus bus;
		return bus;
	}

	[[nodiscard]] static QueryBus& StatisticsQuery() noexcept {
		static QueryBus bus;
		return bus;
	}
	[[nodiscard]] static QueryBus& DebugQuery() noexcept {
		static QueryBus bus;
		return bus;
	}
};

namespace Debug::Event {
	struct FPS final {
		uInt Count;
	};
	struct OnChangedFPS final {
		uInt Count;
	};
}
