#include <UI/UISystem.h>
#include <World/Components/RenderComponents.h>

#include <algorithm>
#include <unordered_set>

void UISystem::Resize(const Nt::Float2D& contextSize) {
	if (m_ContextSize == contextSize)
		return;

	m_ContextSize = contextSize;
	for (UIElement& element : m_Elements) {
		const Bool hasText = element.pText != nullptr && element.pText->Font.IsLoaded();

		Nt::Float2D localSize;
		if (element.pTexture != nullptr) {
			Assert(element.pTexture->IsValid(), "Texture not valid");
			localSize = element.pTexture->Get()->GetSize();
		}
		else if (hasText) {
			for (const Char& symbol : element.pText->TextString) {
				const Nt::FTFont::Character& character =
					element.pText->Font.GetCharacter(symbol);

				localSize.x -= Float(character.Size.x);
				localSize.y = std::min<Float>(localSize.y, -Float(character.Size.y));
			}
		}
		else {
			localSize = { 1.f, 1.f };
		}

		if (element.pScale != nullptr) {
			element.pScale->LocalContextSize(localSize);
			element.pScale->ContextSize(contextSize);

			if (hasText) {
				const Nt::Float2D size = element.pTransform->Size() / localSize;
				element.pTransform->Size(size);
			}
		}
		else if (!hasText) {
			element.pTransform->Resize(localSize);
		}
	}
}

void UISystem::RegisterCanvas(Canvas& canvas) {
	for (ObjectPtr& pObject : canvas.GetObjects()) {
		if (pObject->GetType() == ObjectType::Canvas) {
			Canvas* pCanvas = static_cast<Canvas*>(pObject.get());
			RegisterCanvas(*pCanvas);
			continue;
		}

		UIElement element = { };
		element.pTransform = RequireNotNull(pObject->GetComponent<Transform2D>());
		element.pScale = pObject->GetComponent<Scale2D>();
		element.pTexture = pObject->GetComponent<TextureRenderer>();
		element.pText = pObject->GetComponent<Text>();
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
