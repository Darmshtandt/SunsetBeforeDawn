#pragma once

#include <Nt/Core/Utilities.h>
#include <Core/Nt/ClassManager.h>

template <class _Creator>
class IFactory {
public:
	template <class _Ty>
	void Register(_Creator creator) {
		constexpr ClassID classID = Class<_Ty>::ID();

		if (m_Objects.contains(classID))
			Raise("Already registered: " + ClassManager::Instance().GetName<_Ty>());

		m_Objects[classID] = creator;
	}

	[[nodiscard]] std::vector<std::string> GetIDs() const {
		std::vector<std::string> ids;
		ids.reserve(m_Objects.size());

		for (auto& [id, _] : m_Objects)
			ids.push_back(id);

		return ids;
	}

protected:
	std::unordered_map<ClassID, _Creator> m_Objects;

protected:
	__forceinline auto _GetIterator(const ClassID& id) const {
		const auto it = m_Objects.find(id);
		if (it == m_Objects.cend())
			Raise("Not found: " + ClassManager::Instance().GetName(id));

		return it;
	}
};

template <template <class> class _Ptr, class _Ty, class _Creator>
class FactoryBase : public IFactory<_Creator> {
public:
	using BasePtr = _Ptr<_Ty>;

public:
	[[nodiscard]] static FactoryBase& Instance() noexcept {
		static FactoryBase instance;
		return instance;
	}

	[[nodiscard]] BasePtr Create(const ClassID& id) const {
		return this->_GetIterator(id)->second();
	}

	template <class _U>
	[[nodiscard]] BasePtr Create() const {
		return this->_GetIterator(Class<_U>::ID())->second();
	}

	[[nodiscard]] std::unordered_map<ClassID, BasePtr> CreateAll() const {
		std::unordered_map<ClassID, BasePtr> objects;
		objects.reserve(this->m_Objects.size());

		for (auto& [id, creator] : this->m_Objects)
			objects[id] = creator();

		return objects;
	}
};

template <class _Ty>
struct FactoryTraits {
	using Ptr = _Ty*;

	template <class... _Args>
	static Ptr Create(_Args&&... args) {
		return new _Ty(std::forward<_Args>(args)...);
	}
};

template <class _Ty>
struct FactoryTraits<std::shared_ptr<_Ty>> {
	template <class... _Args>
	static std::shared_ptr<_Ty> Create(_Args&&... args) {
		return std::make_shared<_Ty>(std::forward<_Args>(args)...);
	}
};

template <class _Ty>
struct FactoryTraits<std::unique_ptr<_Ty>> {
	template <class... _Args>
	static std::unique_ptr<_Ty> Create(_Args&&... args) {
		return std::make_unique<_Ty>(std::forward<_Args>(args)...);
	}
};