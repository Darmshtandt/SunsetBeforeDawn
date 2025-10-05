#include <UI/UISystem.h>
#include <Objects/Components/RenderComponents.h>

void UISystem::Resize(const Nt::Float2D& contextSize) {
	if (m_ContextSize == contextSize)
		return;

	m_ContextSize = contextSize;
	for (UIElement& element : m_Elements) {
		Nt::Float2D localSize;
		if (element.pTexture != nullptr) {
			Assert(element.pTexture->Handler.IsValid(), "Texture not valid");
			localSize = element.pTexture->Handler.Get()->GetSize();
		}

		if (element.pScale != nullptr) {
			element.pScale->LocalContextSize(localSize);
			element.pScale->ContextSize(contextSize);
		}
		else {
			element.pTransform->Resize(localSize);
		}

		if (element.pLayout == nullptr)
			continue;

		element.pTransform->Translate(element.pLayout->Padding);
	}
}

void UISystem::RegisterCanvas(Canvas& canvas) {
	for (ObjectPtr& pObject : canvas.GetObjects()) {
		if (pObject->GetType() == ObjectType::Canvas) {
			Canvas* pCanvas = static_cast<Canvas*>(pObject.get());
			RegisterCanvas(*pCanvas);
			continue;
		}

		UIElement element;
		element.pTransform = RequireNotNull(pObject->GetComponent<Transform2D>());
		element.pLayout = pObject->GetComponent<Layout2D>();
		element.pScale = pObject->GetComponent<Scale2D>();
		element.pTexture = pObject->GetComponent<TextureComponent>();
		element.pObject = pObject.get();
		m_Elements.emplace_back(element);
	}
}

void UISystem::UnregisterCanvas(const Canvas& canvas) {
	const std::vector<ObjectPtr>& canvasObjects = canvas.GetObjects();

	std::unordered_set<GameObject*> canvasSet;
	for (const ObjectPtr& pObject : canvasObjects)
		canvasSet.insert(pObject.get());

	auto compare = 
		[&canvasSet] (const UIElement& element) {
			return canvasSet.contains(element.pObject);
		};

	m_Elements.erase(
		std::ranges::remove_if(m_Elements, compare).begin(),
		m_Elements.end()
	);
}

void UISystem::Clear() noexcept {
	m_Elements.clear();
}

void UISystem::Update(const Float& deltaTime) {
}
