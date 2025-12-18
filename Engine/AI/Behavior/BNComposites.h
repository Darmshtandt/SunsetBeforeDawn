#pragma once

#include <Engine/AI/Behavior/BehaviorNode.h>
#include <functional>

struct BehaviorSelectorNode final : IBehaviorNode {
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
	inline static BNRegistrar<BehaviorSelectorNode> m_Registrar = { "Selector" };
	std::vector<BehaviorNodePtr> m_Nodes;
	uInt m_RunningNodeIndex = 0;
};

struct BehaviorSequenceNode final : IBehaviorNode {
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
	inline static BNRegistrar<BehaviorSequenceNode> m_Registrar = { "Sequence" };
	std::vector<BehaviorNodePtr> m_Nodes;
	uInt m_RunningNodeIndex = 0;
};

struct BehaviorActionNode final : IBehaviorNode {
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
	inline static BNRegistrar<BehaviorActionNode> m_Registrar = { "Condition" };
	std::function<ActionFunc> m_ActionFunc;
};