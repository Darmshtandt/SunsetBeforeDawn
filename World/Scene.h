#pragma once

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
	Scene() noexcept = default;
	~Scene() noexcept = default;

	void Initialize();
	void Generate();

	template <class _Ty> requires std::is_base_of_v<GameObject, _Ty>
	void AddObject(const Nt::Float3D& position) {
		ObjectPtr pObject = ObjectFactory::Instance().Create<_Ty>();

		if (position.LengthSquare() != 0.f) {
			auto* pTransform =
				RequireNotNull(pObject->GetComponent<Transform3D>());
			pTransform->Position(position);
		}

		const auto* pHandler = pObject->GetComponent<Handler>();
		if (pHandler != nullptr && pHandler->Function)
			m_Handlers.emplace_back(pHandler);

		m_Dispatcher.Emmit<OnAdd>({ pObject.get() });
		m_Objects.emplace_back(std::move(pObject));
	}

	template <class _Ty> requires std::is_base_of_v<GameObject, _Ty>
	void RemoveObject(const NotNull<std::shared_ptr<_Ty>>& pObject) {
		m_Dispatcher.Emmit<OnRemove>({ pObject.Get().get() });

		const auto* pHandler = pObject->template GetComponent<Handler>();
		if (pHandler != nullptr && pHandler->Function)
			std::erase(m_Handlers, pHandler);

		std::erase(m_Objects, pObject.Get());
	}

	void Clear();

	void Update(const Float& deltaTime);

	[[nodiscard]] EventBus* GetDispatcherPtr() noexcept;
	[[nodiscard]] const std::vector<ObjectPtr>& GetObjects() const noexcept;

private:
	EventBus m_Dispatcher;
	std::weak_ptr<Landscape> m_pLandscape;

	std::vector<const Handler*> m_Handlers;
	std::vector<ObjectPtr> m_Objects;
};