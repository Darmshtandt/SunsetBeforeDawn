#pragma once

#include <Engine/AI/AIContext.h>

// BN - BehaviorNode
enum class BNStatus {
	Failed,
	Successful,
	Running
};

struct IBehaviorNode {
	virtual ~IBehaviorNode() noexcept = default;

	[[nodiscard]]
	virtual BNStatus Tick(const AIContext& context, Float deltaTime) = 0;
	virtual IBehaviorNode* AddNode(const std::string& name) = 0;
};

using BehaviorNodePtr = std::unique_ptr<IBehaviorNode>;

// BN - BehaviorNode
class BNFactory final {
public:
	using Creator = BehaviorNodePtr(*)();

public:
	[[nodiscard]] static BNFactory& Instance() noexcept {
		static BNFactory instance;
		return instance;
	}

	void Register(Creator creator, const std::string& name) {
		if (m_Objects.contains(name))
			Raise("Already registered: " + name);

		m_Objects[name] = creator;
	}

	[[nodiscard]] BehaviorNodePtr Create(const std::string& name) const {
		const auto it = m_Objects.find(name);
		if (it == m_Objects.cend())
			Raise("Not found: " + name);

		return it->second();
	}


	[[nodiscard]] std::vector<std::string> GetNames() const {
		std::vector<std::string> names;
		names.reserve(m_Objects.size());

		for (auto& [name, _] : m_Objects)
			names.push_back(name);

		return names;
	}

protected:
	std::unordered_map<std::string, Creator> m_Objects;
};

// BN - BehaviorNode
template <class _Ty>
struct BNRegistrar final {
	BNRegistrar(const std::string& name) {
		BNFactory::Instance().Register([] () -> BehaviorNodePtr {
			return std::make_unique<_Ty>();
			}, name);
	}
};