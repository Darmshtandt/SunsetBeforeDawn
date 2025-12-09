#pragma once

#include <Engine/AI/IBehavior.h>
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
struct BNodeRegistrar final {
	BNodeRegistrar(const std::string& name) {
		BNFactory::Instance().Register([] () -> BehaviorNodePtr {
			return std::make_unique<_Ty>();
			}, name);
	}
};


struct BehaviorSelectorNode : IBehaviorNode {
	~BehaviorSelectorNode() noexcept override = default;

	[[nodiscard]]
	BNStatus Tick(const AIContext& context, Float deltaTime) override {
		for (uInt i = m_RunningNodeIndex; i < m_Nodes.size(); ++i) {
			const BNStatus nodeBehaviorStatus = m_Nodes[i]->Tick(context, deltaTime);
			if (nodeBehaviorStatus == BNStatus::Successful) {
				m_RunningNodeIndex = 0;
				return BNStatus::Successful;
			}
			if (nodeBehaviorStatus == BNStatus::Running) {
				m_RunningNodeIndex = i;
				return BNStatus::Running;
			}
		}

		m_RunningNodeIndex = 0;
		return BNStatus::Failed;
	}

	IBehaviorNode* AddNode(const std::string& name) override {
		m_Nodes.emplace_back(BNFactory::Instance().Create(name));
		return m_Nodes.back().get();
	}

private:
	inline static BNodeRegistrar<BehaviorSelectorNode> m_Registrar = { "Selector" };
	std::vector<BehaviorNodePtr> m_Nodes;
	uInt m_RunningNodeIndex = 0;
};

struct BehaviorSequenceNode : IBehaviorNode {
	~BehaviorSequenceNode() noexcept override = default;

	[[nodiscard]]
	BNStatus Tick(const AIContext& context, Float deltaTime) override {
		for (uInt i = m_RunningNodeIndex; i < m_Nodes.size(); ++i) {
			const BNStatus nodeBehaviorStatus = m_Nodes[i]->Tick(context, deltaTime);
			if (nodeBehaviorStatus == BNStatus::Failed) {
				m_RunningNodeIndex = 0;
				return BNStatus::Failed;
			}
			if (nodeBehaviorStatus == BNStatus::Running) {
				m_RunningNodeIndex = i;
				return BNStatus::Running;
			}
		}

		m_RunningNodeIndex = 0;
		return BNStatus::Successful;
	}

	IBehaviorNode* AddNode(const std::string& name) override {
		m_Nodes.emplace_back(BNFactory::Instance().Create(name));
		return m_Nodes.back().get();
	}

private:
	inline static BNodeRegistrar<BehaviorSequenceNode> m_Registrar = { "Sequence" };
	std::vector<BehaviorNodePtr> m_Nodes;
	uInt m_RunningNodeIndex = 0;
};

struct BehaviorActionNode : IBehaviorNode {
	using ActionFunc = BNStatus(const AIContext& context, Float deltaTime);

	~BehaviorActionNode() noexcept override = default;

	[[nodiscard]]
	BNStatus Tick(const AIContext& context, Float deltaTime) override {
		if (!m_ActionFunc)
			return BNStatus::Successful;
		return m_ActionFunc(context, deltaTime);
	}

	void SetActionFunction(std::function<ActionFunc> func) noexcept {
		m_ActionFunc = std::move(func);
	}

	IBehaviorNode* AddNode(const std::string& name) override {
		Raise("The Condition does not contained nodes");
	}

private:
	inline static BNodeRegistrar<BehaviorActionNode> m_Registrar = { "Condition" };
	std::function<ActionFunc> m_ActionFunc;
};