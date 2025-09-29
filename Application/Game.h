#pragma once

#include <World/Scene.h>
#include <Helpers/DebugSpectator.h>
#include <Controllers/DebugController.h>
#include <Controllers/RotatableController.h>
#include <Physics/PhysicsSystem.h>

class Game {
public:
	using ControllerMap = std::unordered_map<ControllerID, ControllerPtr>;
	using ControllerSet = std::unordered_set<IController*>;

	struct OnRegisterInput final {
		IController* pController;
	};
	struct OnUnregisterInput final {
		IController* pController;
	};

public:
	Game();
	Game(const Game&) = delete;
	Game(Game&&) noexcept = default;
	~Game();

	Game& operator = (const Game&) = delete;
	Game& operator = (Game&&) noexcept = default;

	void Initialize(Nt::Shader* pShader);

	void Uninitialize();

	template <class _Controller, class _Object>
	void AddController(NotNull<_Object*> object) {
		ControllerPtr pController = std::make_unique<_Controller>(object);

		EventBus::Instance().Emmit<OnRegisterInput>({ pController.get() });

		m_Controllers[Class<_Controller>::ID()] = std::move(pController);
	}

	template <class _Controller>
	void RemoveController() {
		const ControllerID id = Class<_Controller>::ID();
		DeativateController(id);

		const auto it = m_Controllers.find(id);
		if (it != m_Controllers.end()) {
			IController* pController = it->second.get();
			EventBus::Instance().Emmit<OnUnregisterInput>({ pController });
			m_Controllers.erase(it);
		}
	}

	void Update(const Float& deltaTime);
	void Render(Nt::Renderer* pRenderer) const;

	[[nodiscard]] CameraObject* GetCameraPtr() noexcept;

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
	PhysicsSystem m_PhysicsSystem;
	BehaviorSystem m_BehaviorSystem;
	TargetingSystem m_TargetingSystem;
	ControllerMap m_Controllers;
	ControllerSet m_ActiveControllers;

	CameraObject m_Camera;
	DebugSpectator m_Spectator;
	Scene m_Scene;

private:
	ControllerPtr& _GetController(const ControllerID& id) {
		auto it = m_Controllers.find(id);
		Assert(it != m_Controllers.cend(), "Unknown controller");
		return it->second;
	}

	void _HandleEventOnAdd(const Scene::OnAdd& event) {
		if (event.pObject->GetType() == ObjectType::Player) {
			Player* pPlayer = static_cast<Player*>(event.pObject);

			AddController<MovableController, Movable>(pPlayer);
			AddController<RotatableController, Movable>(pPlayer);

			ActivateController<MovableController>();
			ActivateController<RotatableController>();
			m_Camera.SetTarget(pPlayer);
		}
		else if (event.pObject->GetType() == ObjectType::Creature) {
			Creature* pCreature = static_cast<Creature*>(event.pObject);

			m_BehaviorSystem.EvaluateBehavior(pCreature);
			m_TargetingSystem.RegisterCreature(m_Scene, *pCreature);
		}

		m_PhysicsSystem.AddObject(event.pObject);		
	}
	void _HandleEventOnRemove(const Scene::OnRemove& event) {
		if (event.pObject->GetType() == ObjectType::Player) {
			RemoveController<MovableController>();
			RemoveController<RotatableController>();
			m_Camera.SetTarget(nullptr);
		}

		m_PhysicsSystem.RemoveObject(event.pObject);
	}
	void _HandleEventOnClear(const Scene::OnClear& event) {
		(void)event;

		m_ActiveControllers.clear();
		m_Controllers.clear();
		m_PhysicsSystem.Clear();
		m_Camera.SetTarget(nullptr);		
	}
};
