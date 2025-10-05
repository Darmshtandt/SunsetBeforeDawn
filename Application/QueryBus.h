#pragma once

#include <ClassManager.h>
#include <Nt/Core/Utilities.h>
#include <functional>

class QueryBus final {
public:
	template <class _Request>
	using Function = std::function<_Request()>;
	using VoidPtr = std::shared_ptr<void>;
	using RequestMap = std::unordered_map<ClassID, VoidPtr>;

public:
	QueryBus() noexcept = default;
	QueryBus(const QueryBus&) noexcept = default;
	QueryBus(QueryBus&&) noexcept = default;
	~QueryBus() noexcept = default;

	QueryBus& operator = (const QueryBus&) noexcept = default;
	QueryBus& operator = (QueryBus&&) noexcept = default;

	template <class _Request, class _Func>
	void Subscribe(_Func&& func) {
		const ClassID& id = Class<_Request>::ID();
		Assert(!m_RequestMap.contains(id), "Request already subscribed");

		m_RequestMap[id] = std::make_shared<Function<_Request>>(
			Function<_Request>(std::forward<_Func>(func)));
	}

	template <class _Request>
	void Unsubscribe() {
		m_RequestMap.erase(_GetIterator<_Request>());
	}

	void Clear() {
		m_RequestMap.clear();
	}

	template <class _Request>
	_Request Request() const {
		return _GetFunction<_Request>()();
	}

	template <class _Request>
	[[nodiscard]] Bool Has() const noexcept {
		return m_RequestMap.contains(Class<_Request>::ID());
	}

private:
	RequestMap m_RequestMap;

private:
	template <class _Request>
	RequestMap::const_iterator _GetIterator() const {
		const ClassID& id = Class<_Request>::ID();

		const auto it = m_RequestMap.find(id);
		Assert((it != m_RequestMap.cend()), "Unknown request");
		return it;
	}

	template <class _Request>
	Function<_Request>& _GetFunction() const {
		const auto it = _GetIterator<_Request>();
		return *std::static_pointer_cast<Function<_Request>>(it->second);
	}
};