#pragma once

#include <ClassManager.h>
#include <Nt/Core/NtTypes.h>
#include <unordered_map>
#include <functional>
#include <memory>

using EventID = uInt;

template <class _Ty>
struct EventType {
	[[nodiscard]] static constexpr EventID ID() noexcept {
		return Class<_Ty>::ID();
	}
};

struct Subscription {
	EventID EventTypeID;
	uInt Index;
};

class EventBus {
public:
	template <class _Event>
	using Function = std::function<void(const _Event&)>;

	template <class _Event>
	using Container = std::vector<Function<_Event>>;

public:
	EventBus() noexcept = default;
	EventBus(const EventBus&) = default;
	EventBus(EventBus&&) noexcept = default;
	~EventBus() noexcept = default;

	EventBus& operator = (const EventBus&) = default;
	EventBus& operator = (EventBus&&) noexcept = default;

	template <class _Event, class _Func>
	Subscription Subscribe(_Func&& func) {
		Container<_Event>& container = _GetOrCreateContainer<_Event>();
		const Subscription sub = { EventType<_Event>::ID(), container.size() };

		container.emplace_back(Function<_Event>(std::forward<_Func>(func)));
		return sub;
	}

	template <class _Event>
	void Unsubscribe(const Subscription& sub) {
		if (sub.EventTypeID == EventType<_Event>::ID())
			return;

		Container<_Event>& container = _GetContainer<_Event>();
		if (sub.Index < container.size())
			container[sub.Index] = nullptr;
	}

	template <class _Event>
	void Emmit(const _Event& event) {
		Container<_Event>& container = _GetContainer<_Event>();
		for (Function<_Event>& func : container) {
			if (func)
				func(event);
		}
	}

private:
	std::unordered_map<EventID, std::shared_ptr<void>> m_EventMap;

private:
	template <class _Event>
	[[nodiscard]] Container<_Event>& _GetOrCreateContainer() {
		EventID id = EventType<_Event>::ID();

		auto it = m_EventMap.find(id);
		if (it == m_EventMap.end()) {
			auto pointer = std::make_shared<Container<_Event>>();
			m_EventMap[id] = pointer;
			return *pointer;
		}

		return *std::static_pointer_cast<Container<_Event>>(it->second);
	}

	template <class _Event>
	[[nodiscard]] Container<_Event>& _GetContainer() {
		auto it = m_EventMap.find(EventType<_Event>::ID());
		if (it == m_EventMap.end()) {
			static Container<_Event> empty;
			return empty;
		}

		return *std::static_pointer_cast<Container<_Event>>(it->second);
	}
};