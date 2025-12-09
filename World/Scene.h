#pragma once

#include <variant>
#include <Core/EventBus.h>
#include <World/Objects/GameObject.h>

class Landscape;

class Scene final {
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

public:
	Scene(NotNull<EventBus*> pBus) noexcept;
	~Scene() noexcept = default;

	void Initialize();
	void Generate();

	void AddObject(const Nt::Float3D& position, ClassID id);
	void RemoveObject(GameObject& object);

	template <class... _Components>
	std::vector<std::tuple<_Components*...>> GetComponents() {
		using Object = std::tuple<_Components*...>;

		std::vector<Object> objects;
		for (ObjectPtr& pObject : m_Objects) {
			Object object = pObject->GetComponentsTuple<_Components>();

			const Bool isNull =
				std::apply([] (auto... pComponents) {
						return ((pComponents == nullptr) && ...);
					}, object);

			if (isNull)
				continue;

			objects.emplace_back(object);
		}

		return objects;
	}

	template <class _Ty> requires std::is_base_of_v<GameObject, _Ty>
	void AddObject(const Nt::Float3D& position) {
		AddObject(position, Class<_Ty>::ID());
	}

	void Clear();

	void Update(const Float& deltaTime);

	[[nodiscard]] const std::vector<ObjectPtr>& GetObjects() const noexcept;

private:
	EventBus* m_pDispatcher;
	std::weak_ptr<Landscape> m_pLandscape;

	std::vector<const Handler*> m_Handlers;
	std::vector<ObjectPtr> m_Objects;
};