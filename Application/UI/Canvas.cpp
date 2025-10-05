#include <UI/Canvas.h>

Canvas::Canvas(const ClassID& id) noexcept :
	GameObject(id, ObjectType::Canvas)
{
}

GameObject* Canvas::AddObject(ObjectPtr pObject) {
	GameObject* objectRawPtr = pObject.get();
	m_Objects.emplace_back(std::move(pObject));
	return objectRawPtr;
}

void Canvas::RemoveObject(const GameObject& object) {
	std::erase_if(m_Objects, [&object] (const ObjectPtr& pObject) {
		return pObject.get() == &object;
	});
}

void Canvas::Clear() {
	m_Objects.clear();
}

std::vector<ObjectPtr>& Canvas::GetObjects() noexcept {
	return m_Objects;
}

const std::vector<ObjectPtr>& Canvas::GetObjects() const noexcept {
	return m_Objects;
}