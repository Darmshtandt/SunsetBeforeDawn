#pragma once

#include <Engine/AI/BehaviorNode.h>
#include <Core/Interval.h>
#include <queue>

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

namespace Temporary {
	class BNPatrol final : public IBehaviorNode {
	private:
		enum class State : Byte {
			Stay,
			Walking
		};

	public:
		BNPatrol() noexcept :
			m_ChangeStateInterval(1500, 1500, [this] () { _ChangeState(); })
		{
		}
		~BNPatrol() noexcept override = default;

		[[nodiscard]]
		BNStatus Tick(const AIContext& context, Float deltaTime) override {
			m_ChangeStateInterval.Update();
			if (m_State == State::Stay)
				return BNStatus::Successful;

			context.pMovementController->MoveInDirection(m_Direction);
			context.pMovementController->LookInDirection(m_Direction, deltaTime * 4.f);

			return BNStatus::Successful;
		}
		IBehaviorNode* AddNode(const std::string& name) override {
			Raise("The Condition does not contained nodes");
		}

	private:
		Nt::Float3D m_Direction;
		Interval m_ChangeStateInterval;
		State m_State = State::Stay;

		inline static BNodeRegistrar<BNPatrol> m_Registrar = { "Patrol" };

	private:
		void _ChangeState() noexcept {
			if (std::rand() % 2 == 0) {
				m_State = State::Walking;
				m_Direction = Nt::Float3D(
					std::rand() + 1.f, 0.f, std::rand() + 1.f).GetNormalize();
			}
			else {
				m_State = State::Stay;
			}
		}
	};

	struct BNPursue final : IBehaviorNode {
		~BNPursue() noexcept override = default;

		[[nodiscard]]
		BNStatus Tick(const AIContext& context, Float deltaTime) override {
			if (!context.pAttackController->HasTarget())
				return BNStatus::Failed;

			Route3D* pRoute = context.pMovementController->GetRoute();
			if (!pRoute->HasMarker())
				return BNStatus::Successful;

			if (pRoute->IsReachedMarker(deltaTime)) {
				pRoute->PopMarker();
				if (!pRoute->HasMarker())
					return BNStatus::Successful;
			}

			const Nt::Float3D direction = -pRoute->DirectionToMarker();
			context.pMovementController->MoveInDirection(direction);
			context.pMovementController->LookInDirection(direction, deltaTime * 4.f);

			return BNStatus::Running;
		}
		IBehaviorNode* AddNode(const std::string& name) override {
			Raise("The Condition does not contained nodes");
		}

	private:
		inline static BNodeRegistrar<BNPursue> m_Registrar = { "Pursue" };
	};

	struct BNAttack final : IBehaviorNode {
		~BNAttack() noexcept override = default;

		[[nodiscard]]
		BNStatus Tick(const AIContext& context, Float deltaTime) override {
			return BNStatus::Running;
		}
		IBehaviorNode* AddNode(const std::string& name) override {
			Raise("The Condition does not contained nodes");
		}

	private:
		inline static BNodeRegistrar<BNAttack> m_Registrar = { "Attack" };
	};

	//struct BTCreator final {
	//	static BehaviorTree CreateZombieTree() {
	//		std::stack<IBehaviorNode*> bnStack;

	//		BehaviorTree behaviorTree;
	//		bnStack.push(behaviorTree.SetRootNode<BehaviorSelectorNode>());

	//		bnStack.push(bnStack.top()->AddNode("Sequence"));
	//		bnStack.top()->AddNode("Pursue");
	//		bnStack.top()->AddNode("Attack");
	//		bnStack.pop();

	//		bnStack.top()->AddNode("Patrol");

	//		return behaviorTree;
	//	}
	//};
}