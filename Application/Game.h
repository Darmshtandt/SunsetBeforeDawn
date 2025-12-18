#pragma once

#include <World/Scene.h>
#include <Engine/IController.h>
#include <Engine/ISystem.h>
#include <unordered_set>
#include <forward_list>

class AISystem;
class PhysicsSystem;
class MovementSystem;
class CombatSystem;

class Game {
public:
	using ControllerMap = std::unordered_map<ControllerID, ControllerPtr>;
	using ControllerSet = std::unordered_set<IController*>;

	struct OnAddToScene final {
		GameObject* pObject;
	};
	struct OnRemoveFromScene final {
		GameObject* pObject;
	};
	struct OnClearScene final
	{
	};

public:
	explicit Game(NotNull<EventBus*> pBus);
	Game(const Game&) = delete;
	Game(Game&&) noexcept = default;
	~Game() noexcept;

	Game& operator = (const Game&) = delete;
	Game& operator = (Game&&) noexcept = default;

	void Initialize();

	template <class _Controller, class _Object>
	void AddController(NotNull<_Object*> object) {
		ControllerPtr pController = std::make_unique<_Controller>(object);

		m_Controllers[Class<_Controller>::ID()] = std::move(pController);
	}

	template <class _Controller>
	void RemoveController() {
		const ControllerID id = Class<_Controller>::ID();
		DeativateController(id);

		const auto it = m_Controllers.find(id);
		if (it != m_Controllers.end()) {
			IController* pController = it->second.get();
			m_Controllers.erase(it);
		}
	}

	void Update(const Float& deltaTime);

	[[nodiscard]] Scene& GetScene() noexcept;

	template <class _Controller>
	void ActivateController() {
		ActivateController(Class<_Controller>::ID());
	}

	template <class _Controller>
	void DeativateController(const ControllerID& id) {
		DeativateController(Class<_Controller>::ID());
	}

	template <class _Controller>
	void ToggleController(const ControllerID& id) {
		ToggleController(Class<_Controller>::ID());
	}

	void ActivateController(const ControllerID& id);
	void DeativateController(const ControllerID& id);
	void ToggleController(const ControllerID& id);

private:
	std::forward_list<SystemPtr> m_Systems;
	ControllerMap m_Controllers;
	ControllerSet m_ActiveControllers;

	Scene m_Scene;
	EventBus* m_pDispatcher;
	EventBus m_SceneDispatcher;

private:
	ControllerPtr& _GetController(const ControllerID& id);

	void _HandleEventOnAdd(const Scene::OnAdd& event);
	void _HandleEventOnRemove(const Scene::OnRemove& event);
	void _HandleEventOnClear(const Scene::OnClear& event);
};
