#pragma once

#include <EventBus.h>
#include <Objects/GameObject.h>

struct IAction;
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

	using ActionPtr = std::shared_ptr<IAction>;
	using ActionList = std::list<ActionPtr>;

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

		m_Dispatcher.Emmit<OnAdd>({ pObject.get() });

		if (pObject->GetType() == ObjectType::Static)
			m_Statics.emplace_back(std::move(pObject));
		else
			m_Dynamic.emplace_back(std::move(pObject));
	}

	template <class _Ty> requires std::is_base_of_v<GameObject, _Ty>
	void RemoveObject(const NotNull<std::shared_ptr<_Ty>>& pObject) {
		m_Dispatcher.Emmit<OnRemove>({ pObject.Get().get() });

		if (pObject->GetType() == ObjectType::Static)
			std::erase(m_Statics, pObject.Get());
		else
			std::erase(m_Dynamic, pObject.Get());
	}

	void Clear();

	void AddAction(const NotNull<ActionPtr>& pAction);
	void RemoveAction(const NotNull<ActionPtr>& pAction);

	void Update(const Float& deltaTime);

	[[nodiscard]] EventBus* GetDispatcherPtr() noexcept;
	[[nodiscard]] const std::vector<ObjectPtr>& GetDynamicObjects() const noexcept;
	[[nodiscard]] const std::vector<ObjectPtr>& GetStaticObjects() const noexcept;

private:
	EventBus m_Dispatcher;
	std::weak_ptr<Landscape> m_pLandscape;

	ActionList m_ActionList;
	std::vector<ObjectPtr> m_Dynamic;
	std::vector<ObjectPtr> m_Statics;
};