#pragma once

#include <EventBus.h>
#include <Objects/GameObject.h>

class Canvas : public GameObject {
protected:
	explicit Canvas(const ClassID& id) noexcept;

public:
	Canvas() = delete;
	Canvas(const Canvas&) noexcept = default;
	Canvas(Canvas&&) noexcept = default;
	~Canvas() noexcept override = default;

	Canvas& operator = (const Canvas&) noexcept = default;
	Canvas& operator = (Canvas&&) noexcept = default;

	template <class _Ty> requires std::is_base_of_v<GameObject, _Ty>
	_Ty* AddObject() {
		return static_cast<_Ty*>(AddObject(std::move(std::make_shared<_Ty>())));
	}

	GameObject* AddObject(ObjectPtr pObject);
	void RemoveObject(const GameObject& object);
	void Clear();

	[[nodiscard]] std::vector<ObjectPtr>& GetObjects() noexcept;
	[[nodiscard]] const std::vector<ObjectPtr>& GetObjects() const noexcept;

private:
	std::vector<ObjectPtr> m_Objects;
};