#pragma once

#include <EventBus.h>
#include <Actions/IAction.h>

#include <World/Biomes/AllBiomes.h>
#include <World/Landscape.h>

#include <Objects/Actors/Player.h>
#include <Objects/Creatures/AllCreatures.h>

#include <Objects/NPC/Behaviors/BehaviorSystem.h>
#include <Objects/NPC/TargetingSystem.h>
#include <Objects/Interactives/Interactive.h>

class Scene {
public:
	struct OnAdd final {
		GameObject* pObject;
	};
	struct OnRemove final {
		GameObject* pObject;
	};
	struct OnClear final
	{
	};

	using ActionList = std::list<ActionPtr>;

public:
	Scene() noexcept = default;

	void Initialize(Nt::Shader* pShader);

	void Generate();

	template <class _Ty> requires std::is_base_of_v<GameObject, _Ty>
	void Add(const Nt::Float3D& position) {
		std::shared_ptr<_Ty> pObject = std::dynamic_pointer_cast<_Ty>(
			ObjectFactory::Instance().Create<_Ty>());
		pObject->SetPosition(position);

		EventBus::Instance().Emmit<OnAdd>({ pObject.get() });

		if constexpr (std::is_base_of_v<Player, _Ty>)
			m_Players.emplace_back(std::move(pObject));
		else if constexpr (std::is_base_of_v<Interactive, _Ty>)
			m_Interactives.emplace_back(std::move(pObject));
		else if constexpr (std::is_base_of_v<Creature, _Ty>)
			m_Creatures.emplace_back(std::move(pObject));
		else
			m_Statics.emplace_back(std::move(pObject));
	}

	template <class _Ty> requires std::is_base_of_v<GameObject, _Ty>
	void Remove(const NotNull<std::shared_ptr<_Ty>>& pObject) {
		EventBus::Instance().Emmit<OnRemove>({ pObject.Get().get() });

		if constexpr (std::is_base_of_v<Player, _Ty>)
			std::erase(m_Players, pObject.Get());
		else if constexpr (std::is_base_of_v<Creature, _Ty>)
			std::erase(m_Creatures, pObject.Get());
		else if constexpr (std::is_base_of_v<Interactive, _Ty>)
			std::erase(m_Interactives, pObject.Get());
		else
			std::erase(m_Statics, pObject.Get());
	}

	void Clear();

	void AddAction(const NotNull<ActionPtr>& pAction);
	void RemoveAction(const NotNull<ActionPtr>& pAction);

	void Update(const Float& deltaTime);
	void Render(NotNull<Nt::Renderer*> pRenderer) const;

	[[nodiscard]] const std::vector<PlayerPtr>& GetAllPlayers() const noexcept;
	[[nodiscard]] const std::vector<CreaturePtr>& GetAllCreatures() const noexcept;
	[[nodiscard]] const std::vector<InteractivePtr>& GetAllInteractives() const noexcept;
	[[nodiscard]] const std::vector<ObjectPtr>& GetAllStatics() const noexcept;

private:
	Nt::Shader* m_pShader = nullptr;
	std::weak_ptr<Landscape> m_pLandscape;

	ActionList m_ActionList;
	std::vector<PlayerPtr> m_Players;
	std::vector<CreaturePtr> m_Creatures;
	std::vector<InteractivePtr> m_Interactives;
	std::vector<ObjectPtr> m_Statics;
};