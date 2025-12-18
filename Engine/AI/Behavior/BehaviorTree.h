#pragma once

#include <Engine/AI/Behavior/BehaviorNode.h>

class BehaviorTree final {
public:
	BehaviorTree() noexcept = default;
	~BehaviorTree() noexcept = default;

	void Tick(const AIContext& context, Float deltaTime) {
		if (m_pRoot)
			m_pRoot->Tick(context, deltaTime);
	}

	template <class _Ty> requires std::is_base_of_v<IBehaviorNode, _Ty>
	_Ty* SetRootNode() {
		m_pRoot = std::make_unique<_Ty>();
		return static_cast<_Ty*>(m_pRoot.get());
	}

private:
	std::unique_ptr<IBehaviorNode> m_pRoot;
};